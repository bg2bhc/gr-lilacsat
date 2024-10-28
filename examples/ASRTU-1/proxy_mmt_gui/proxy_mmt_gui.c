#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <libwebsockets.h>
#include <libconfig.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include <signal.h>

#define MAX_PAYLOAD_SIZE 4096 // Adjust as needed

WINDOW *win_hex;
WINDOW *win_config;
WINDOW *win_log;

int reconnect_needed = 0;
char ws_host[256];

int max_y, max_x;

// Structure to hold configuration data
typedef struct
{
    char *zmq_address;
    char *ws_address;
    int ws_port;
    char *sat_name;
    char *proxy_nickname;
    int physical_channel;
    double proxy_long;
    double proxy_alt;
    double proxy_lat;
} ConfigData;

char *get_current_datetime()
{
    time_t t = time(NULL);
    struct tm *tt = localtime(&t);

    // 分配足够的内存来存储格式化后的日期和时间字符串
    char *result = malloc(100);
    if (result != NULL)
    {
        strftime(result, 100, "%Y-%m-%d %H:%M:%S", tt);
    }
    return result;
}

// Function to read and copy configuration
int read_config(const char *filename, ConfigData *config_data)
{
    config_t cfg;
    config_init(&cfg);

    if (!config_read_file(&cfg, filename))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return -1;
    }

    const char *temp_str;
    int temp_int;
    double temp_float;

    // Read and duplicate ZeroMQ address
    if (config_lookup_string(&cfg, "zmq_address", &temp_str))
    {
        config_data->zmq_address = strdup(temp_str);
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    // Read and duplicate WebSocket address
    if (config_lookup_string(&cfg, "ws_address", &temp_str))
    {
        config_data->ws_address = strdup(temp_str);
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    // Read WebSocket port
    if (config_lookup_int(&cfg, "ws_port", &temp_int))
    {
        config_data->ws_port = temp_int;
    }
    else
    {
        config_data->ws_port = 80; // Default port
    }

    // Read and duplicate sat_name
    if (config_lookup_string(&cfg, "sat_name", &temp_str))
    {
        config_data->sat_name = strdup(temp_str);
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    // Read WebSocket port
    if (config_lookup_int(&cfg, "physical_channel", &temp_int))
    {
        config_data->physical_channel = temp_int;
    }
    else
    {
        config_data->physical_channel = 0; // Default port
    }

    // Repeat for other fields
    if (config_lookup_string(&cfg, "proxy_nickname", &temp_str))
    {
        config_data->proxy_nickname = strdup(temp_str);
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    if (config_lookup_float(&cfg, "proxy_long", &temp_float))
    {
        config_data->proxy_long = temp_float;
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    if (config_lookup_float(&cfg, "proxy_alt", &temp_float))
    {
        config_data->proxy_alt = temp_float;
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    if (config_lookup_float(&cfg, "proxy_lat", &temp_float))
    {
        config_data->proxy_lat = temp_float;
    }
    else
    {
        config_destroy(&cfg);
        return -1;
    }

    // Destroy config
    config_destroy(&cfg);
    return 0;
}

// Function to free allocated configuration data
void free_config_data(ConfigData *config_data)
{
    free(config_data->zmq_address);
    free(config_data->ws_address);
    free(config_data->sat_name);
    free(config_data->proxy_nickname);
}

// Global variables for WebSocket
struct lws_context *context;
struct lws *wsi;
int interrupted = 0;

char *json_to_send = NULL;
size_t json_length = 0;
size_t sent_bytes = 0;
int message_pending = 0;

// WebSocket write buffer
#define WRITE_BUFFER_SIZE (LWS_PRE + MAX_PAYLOAD_SIZE)
unsigned char write_buffer[WRITE_BUFFER_SIZE];

// Forward declaration of ws_callback
static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                       void *user, void *in, size_t len);

// WebSocket protocols
static struct lws_protocols protocols[] = {
    {
        "http",
        ws_callback,
        0,
        MAX_PAYLOAD_SIZE,
    },
    {NULL, NULL, 0, 0} // terminator
};

// 主函数
int main(int argc, char **argv)
{
    ConfigData config_data;

    // 初始化ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0); // 隐藏光标

    getmaxyx(stdscr, max_y, max_x);

    // 计算窗口大小
    int upper_height = (max_y * 3) / 4;
    int lower_height = max_y - upper_height;
    int left_width = (max_x * 2) / 3;
    int right_width = max_x - left_width;

    // 创建窗口
    win_hex = newwin(upper_height, left_width, 0, 0);
    win_config = newwin(upper_height, right_width, 0, left_width);
    win_log = newwin(lower_height, max_x, upper_height, 0);

    // 绘制边框
    box(win_hex, 0, 0);
    box(win_config, 0, 0);

    scrollok(win_log, 1);

    // 刷新窗口
    wrefresh(win_hex);
    wrefresh(win_config);
    wrefresh(win_log);

    if (read_config("config.cfg", &config_data) != 0)
    {
        mvwprintw(win_config, 1, 1, "Failed to read configuration.\n");
        wrefresh(win_hex);
        return -1;
    }

    // 在win_config窗口中打印配置信息
    mvwprintw(win_config, 1, 1, "Configuration:");
    mvwprintw(win_config, 2, 1, "ZMQ Address:");
    mvwprintw(win_config, 3, 1, "   %s", config_data.zmq_address);
    mvwprintw(win_config, 4, 1, "WebSocket Address:");
    mvwprintw(win_config, 5, 1, "   %s", config_data.ws_address);
    mvwprintw(win_config, 6, 1, "WebSocket Port: %d", config_data.ws_port);
    mvwprintw(win_config, 7, 1, "Satellite Name: %s", config_data.sat_name);
    mvwprintw(win_config, 8, 1, "Proxy Nickname: %s", config_data.proxy_nickname);
    mvwprintw(win_config, 9, 1, "Proxy Longitude: %f", config_data.proxy_long);
    mvwprintw(win_config, 10, 1, "Proxy Altitude: %f", config_data.proxy_alt);
    mvwprintw(win_config, 11, 1, "Proxy Latitude: %f", config_data.proxy_lat);
    wrefresh(win_config);

    // 初始化ZeroMQ
    void *zmq_context = zmq_ctx_new();
    void *subscriber = zmq_socket(zmq_context, ZMQ_SUB);
    zmq_connect(subscriber, config_data.zmq_address);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

    // 初始化WebSocket上下文
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    context = lws_create_context(&info);

    if (!context)
    {
        wprintw(win_log, "[%s] Failed to create WebSocket context.\n", get_current_datetime());
        wrefresh(win_log);
        endwin(); // 结束ncurses
        return -1;
    }

    // Setup WebSocket client connection
    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = context;

    // Extract host and port from ws_address
    char ws_host[256];
    char ws_address_copy[256];
    strncpy(ws_address_copy, config_data.ws_address, sizeof(ws_address_copy) - 1);
    ws_address_copy[sizeof(ws_address_copy) - 1] = '\0';

    // Remove "ws://" or "wss://" prefix
    char *address_start = strstr(ws_address_copy, "://");
    if (address_start)
    {
        address_start += 3;
    }
    else
    {
        address_start = ws_address_copy;
    }

    // Separate host and port if specified
    char *colon_ptr = strchr(address_start, ':');
    if (colon_ptr)
    {
        *colon_ptr = '\0';
        strcpy(ws_host, address_start);
        config_data.ws_port = atoi(colon_ptr + 1);
    }
    else
    {
        strcpy(ws_host, address_start);
    }

    ccinfo.address = ws_host;
    ccinfo.port = config_data.ws_port;
    ccinfo.path = "/";
    ccinfo.host = lws_canonical_hostname(context);
    ccinfo.origin = "origin";
    ccinfo.protocol = protocols[0].name;
    ccinfo.ssl_connection = 0; // Set to LCCSCF_USE_SSL if using "wss://"

    wsi = lws_client_connect_via_info(&ccinfo);

    if (!wsi)
    {
        wprintw(win_log, "[%s] WebSocket connection failed.\n", get_current_datetime());
        wrefresh(win_log);
        sleep(5);
        reconnect_needed = 1;
    }

    // 主循环
    while (!interrupted)
    {
        // 处理重新连接逻辑
        if (reconnect_needed)
        {
            wprintw(win_log, "[%s] Attempting to reconnect...\n", get_current_datetime());
            wrefresh(win_log);

            // 设置 WebSocket 客户端连接信息
            struct lws_client_connect_info ccinfo = {0};
            ccinfo.context = context;

            ccinfo.address = ws_host;
            ccinfo.port = config_data.ws_port;
            ccinfo.path = "/";
            ccinfo.host = lws_canonical_hostname(context);
            ccinfo.origin = "origin";
            ccinfo.protocol = protocols[0].name;
            ccinfo.ssl_connection = 0; // 如果使用 "wss://"，请设置为 LCCSCF_USE_SSL

            wsi = lws_client_connect_via_info(&ccinfo);

            if (!wsi)
            {
                wprintw(win_log, "[%s] WebSocket reconnection failed.\n", get_current_datetime());
                wrefresh(win_log);
                sleep(5); // 等待一段时间再尝试
            }
            else
            {
                wprintw(win_log, "[%s] WebSocket reconnected.\n", get_current_datetime());
                wrefresh(win_log);
                reconnect_needed = 0;
            }
        }

        // Poll ZeroMQ with timeout
        zmq_pollitem_t items[] = {
            {subscriber, 0, ZMQ_POLLIN, 0}};
        int rc = zmq_poll(items, 1, 100); // 100 ms timeout

        if (rc == -1)
        {
            wprintw(win_log, "[%s] ZeroMQ poll error.\n", get_current_datetime());
            wrefresh(win_log);
            break;
        }

        if (items[0].revents & ZMQ_POLLIN)
        {
            // 接收数据从ZeroMQ
            char zmq_buffer[MAX_PAYLOAD_SIZE];
            int zmq_size = zmq_recv(subscriber, zmq_buffer, MAX_PAYLOAD_SIZE, 0);

            if (zmq_size == -1)
            {
                wprintw(win_log, "[%s] ZeroMQ receive error.\n", get_current_datetime());
                wrefresh(win_log);
                continue;
            }

            // 在win_hex窗口中打印原始的十六进制数据
            werase(win_hex);
            box(win_hex, 0, 0);
            mvwprintw(win_hex, 1, 1, "Raw Hex Data:");
            int row = 2, col = 1;
            for (int i = 0; i < zmq_size - 10; i++)
            {
                if (col >= left_width - 3)
                {
                    col = 1;
                    row++;
                }
                mvwprintw(win_hex, row, col, "%02X ", (unsigned char)zmq_buffer[i + 10]);
                col += 3;
            }
            wrefresh(win_hex);

            // Build JSON object
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "sat_name", config_data.sat_name);
            cJSON_AddNumberToObject(root, "physical_channel", config_data.physical_channel);
            cJSON_AddStringToObject(root, "proxy_nickname", config_data.proxy_nickname);
            cJSON_AddNumberToObject(root, "proxy_long", config_data.proxy_long);
            cJSON_AddNumberToObject(root, "proxy_alt", config_data.proxy_alt);
            cJSON_AddNumberToObject(root, "proxy_lat", config_data.proxy_lat);

            // Encode raw_data in hex
            char *hex_data = (char *)malloc(zmq_size * 2 + 4);
            sprintf(hex_data, "b\'");
            for (int i = 0; i < zmq_size - 10; i++)
            {
                sprintf(hex_data + i * 2 + 2, "%02x", (unsigned char)zmq_buffer[i + 10]);
            }
            sprintf(hex_data + (zmq_size - 10) * 2 + 2, "\'");
            cJSON_AddStringToObject(root, "raw_data", hex_data);

            // Get current time in milliseconds
            struct timeval tv;
            gettimeofday(&tv, NULL);
            uint64_t millisecondsSinceEpoch =
                (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
            cJSON_AddNumberToObject(root, "proxy_receive_time", millisecondsSinceEpoch);

            // Convert JSON object to string
            if (json_to_send)
            {
                free(json_to_send);
                json_to_send = NULL;
            }
            json_to_send = cJSON_PrintUnformatted(root);
            json_length = strlen(json_to_send);
            sent_bytes = 0;      // Reset sent_bytes
            message_pending = 1; // Indicate that a message is pending to be sent

            // Clean up
            cJSON_Delete(root);
            free(hex_data);

            // 请求一个可写的回调以发送消息
            lws_callback_on_writable(wsi);
        }

        // 服务 libwebsockets 上下文
        int n = lws_service(context, 0);
        if (n < 0)
        {
            wprintw(win_log, "[%s] lws_service error: %d\n", get_current_datetime(), n);
            wrefresh(win_log);
            interrupted = 1;
        }
        signal(SIGWINCH,NULL);
    }

    // 清理
    lws_context_destroy(context);
    zmq_close(subscriber);
    zmq_ctx_destroy(zmq_context);

    // 结束ncurses
    endwin();
}

// Callback function for WebSocket events
static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                       void *user, void *in, size_t len)
{
    switch (reason)
    {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        // Connection established
        wprintw(win_log, "[%s] WebSocket connection established.\n", get_current_datetime());
        wrefresh(win_log);
        break;

    case LWS_CALLBACK_CLIENT_WRITEABLE:
    {
        if (!message_pending)
        {
            // No message to send
            break;
        }

        size_t remaining = json_length - sent_bytes;
        size_t chunk_size = remaining > MAX_PAYLOAD_SIZE ? MAX_PAYLOAD_SIZE : remaining;

        // Prepare the buffer with LWS_PRE padding
        unsigned char *buf = &write_buffer[LWS_PRE];

        // Copy the next chunk of data to send
        memcpy(buf, json_to_send + sent_bytes, chunk_size);

        int write_flags = 0;

        if (sent_bytes == 0)
        {
            // First frame
            if (remaining > chunk_size)
            {
                // More frames will follow
                write_flags = LWS_WRITE_TEXT | LWS_WRITE_NO_FIN;
            }
            else
            {
                // This is the only frame (message fits in one frame)
                write_flags = LWS_WRITE_TEXT;
            }
        }
        else
        {
            // Continuation frames
            if (remaining > chunk_size)
            {
                // More frames will follow
                write_flags = LWS_WRITE_TEXT | LWS_WRITE_CONTINUATION | LWS_WRITE_NO_FIN;
            }
            else
            {
                // Final frame
                write_flags = LWS_WRITE_TEXT | LWS_WRITE_CONTINUATION;
            }
        }

        // Send the data
        int n = lws_write(wsi, buf, chunk_size, write_flags);
        if (n < 0)
        {
            wprintw(win_log, "[%s] Write failed.\n", get_current_datetime());
            wrefresh(win_log);
            interrupted = 1;
            return -1;
        }

        // Update the number of bytes sent
        sent_bytes += chunk_size;

        // If there is more data to send, request another writable callback
        if (sent_bytes < json_length)
        {
            lws_callback_on_writable(wsi);
        }
        else
        {
            // All data sent
            message_pending = 0;
            sent_bytes = 0;
            free(json_to_send);
            json_to_send = NULL;
        }
        break;
    }

    case LWS_CALLBACK_CLIENT_RECEIVE:
    {
        // Data received from the WebSocket server
        wprintw(win_log, "[%s] Received message from server: %.*s\n", get_current_datetime(), (int)len, (char *)in);
        wrefresh(win_log);
        break;
    }

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        wprintw(win_log, "[%s] Connection error.\n", get_current_datetime());
        wrefresh(win_log);
        sleep(5);
        wsi = NULL;
        reconnect_needed = 1;
        break;

    case LWS_CALLBACK_CLIENT_CLOSED:
        wprintw(win_log, "WebSocket connection closed.\n", get_current_datetime());
        wrefresh(win_log);
        wsi = NULL;
        reconnect_needed = 1;
        break;

    default:
        break;
    }
    return 0;
}
