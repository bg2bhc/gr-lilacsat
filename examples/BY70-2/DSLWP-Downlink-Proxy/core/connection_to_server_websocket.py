# coding:utf-8

import Queue
import math
import tornado
import threading
import json
from tornado.websocket import websocket_connect
from tornado import gen, locks
from tornado.ioloop import IOLoop


class WebSocketClient(object):
    def __init__(self, url, name):
        self._url = url
        self._name = name
        self._ws = None
        self.__lock = locks.Lock()
        self._io_loop = tornado.ioloop.IOLoop.current()
        self._queue = Queue.Queue(maxsize=100)  # change queue size from here
        self._time_out = 0
        self._period = None

    @gen.coroutine
    def start(self):
        with (yield self.__lock.acquire()):
            if self._ws is None:
                try:
                    self._ws = yield websocket_connect(self._url, io_loop=self._io_loop)
                except Exception, e:
                    self._time_out += 1
                    if self._time_out >= 10:
                        self._period.change_callback_time(10000)
                else:
                    self.on_message()
            if self._ws:
                self._time_out = 0
                self._period.change_callback_time(1000)
                self.on_connected()

    def _period_start(self):
        if self._period is None:
            self._period = PeriodicCallback(self.start, 1000, io_loop=self._io_loop)
            self._period.start()

    @gen.coroutine
    def on_message(self):
        while True:
            msg = yield self._ws.read_message()
            if msg is None:
                print "[WebSocket] " + self._name + " socket connection closed"
                self._close()
                break
            else:
                message = json.loads(msg)
                print message['message']

    def on_connected(self):
        while self._queue.empty() == False:
            self._ws.write_message(self._queue.get())

    def _close(self):
        if self._ws:
            self._ws.close()
            self._ws = None

    def _restart_now(self):
        if self._ws is None:
            self.start()

    def SendData(self, data):
        self._period_start()
        self._restart_now()
        if not self._queue.full():
            self._queue.put(data)
        else:
            print "[Queue] Websocket queue received maximum, throw away old data"
            self._queue.get()
            self._queue.put(data)

    def is_alive(self):
        return self._ws != None

    def stop(self):
        self._close()
        if self._period and self._period.is_running():
            self._period.stop()


class PeriodicCallback(object):
    def __init__(self, callback, callback_time, io_loop=None):
        self.callback = callback
        if callback_time <= 0:
            raise ValueError("Periodic callback must have a positive callback_time")
        self.callback_time = callback_time
        self.io_loop = io_loop or IOLoop.current()
        self._running = False
        self._timeout = None

    def start(self):
        """Starts the timer."""
        self._running = True
        self._next_timeout = self.io_loop.time()
        self._schedule_next()

    def stop(self):
        """Stops the timer."""
        self._running = False
        if self._timeout is not None:
            self.io_loop.remove_timeout(self._timeout)
            self._timeout = None

    def is_running(self):
        return self._running

    def _run(self):
        if not self._running:
            return
        try:
            return self.callback()
        except Exception:
            self.io_loop.handle_callback_exception(self.callback)
        finally:
            self._schedule_next()

    def _call_io_loop_thread(self):
        self._timeout = self.io_loop.add_timeout(self._next_timeout, self._run)

    def _schedule_next(self):
        if self._running:
            current_time = self.io_loop.time()

            if self._next_timeout <= current_time:
                callback_time_sec = self.callback_time / 1000.0
                self._next_timeout += (math.floor((current_time - self._next_timeout) /
                                                  callback_time_sec) + 1) * callback_time_sec

            self.io_loop.add_callback(self._call_io_loop_thread)

    def change_callback_time(self, callback_time):
        self.callback_time = callback_time


class Websocket_send_thread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        tornado.ioloop.IOLoop.current().start()

    def join(self):
        tornado.ioloop.IOLoop.current().stop()
