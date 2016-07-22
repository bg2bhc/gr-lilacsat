#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Demod LilacSat-1
# Author: WEI Mingchuan, BG2BHC
# Description: LilacSat-1 9600 bps BPSK telemetry demodulator. Tested on GRC 3.7.8.
# Generated: Thu Apr 14 11:40:56 2016
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import audio
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from grc_param import *
from optparse import OptionParser
import lilacsat
import math
import sip
import sys


class demod_lilacsat1(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Demod LilacSat-1")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Demod LilacSat-1")
        try:
             self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
             pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "demod_lilacsat1")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.sps = sps = 5
        self.nfilts = nfilts = 16
        self.alpha = alpha = 0.35
        self.variable_constellation_0 = variable_constellation_0 = digital.constellation_calcdist(([-1, 1]), ([0, 1]), 2, 1).base()
        self.timing_loop_bw = timing_loop_bw = 0.05
        self.samp_rate = samp_rate = 48000
        self.rrc_taps = rrc_taps = firdes.root_raised_cosine(nfilts, nfilts, 1.0/float(sps), alpha, 11*sps*nfilts)
        self.phase_bw = phase_bw = 0.02
        self.fll_bw = fll_bw = 0.01
        self.equalizer_gain = equalizer_gain = 0.05
        self.digigain = digigain = 20

        ##################################################
        # Blocks
        ##################################################
        self.tab = Qt.QTabWidget()
        self.tab_widget_0 = Qt.QWidget()
        self.tab_layout_0 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_0)
        self.tab_grid_layout_0 = Qt.QGridLayout()
        self.tab_layout_0.addLayout(self.tab_grid_layout_0)
        self.tab.addTab(self.tab_widget_0, "Spectrum")
        self.tab_widget_1 = Qt.QWidget()
        self.tab_layout_1 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_1)
        self.tab_grid_layout_1 = Qt.QGridLayout()
        self.tab_layout_1.addLayout(self.tab_grid_layout_1)
        self.tab.addTab(self.tab_widget_1, "Loop")
        self.tab_widget_2 = Qt.QWidget()
        self.tab_layout_2 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_2)
        self.tab_grid_layout_2 = Qt.QGridLayout()
        self.tab_layout_2.addLayout(self.tab_grid_layout_2)
        self.tab.addTab(self.tab_widget_2, "Audio")
        self.top_grid_layout.addWidget(self.tab, 2,0,1,2)
        self._timing_loop_bw_range = Range(0.0, 0.2, 0.01, 0.05, 200)
        self._timing_loop_bw_win = RangeWidget(self._timing_loop_bw_range, self.set_timing_loop_bw, "Time: Bandwidth", "counter_slider", float)
        self.tab_grid_layout_1.addWidget(self._timing_loop_bw_win, 1,0)
        self._phase_bw_range = Range(0.0, 1.0, 0.01, 0.02, 200)
        self._phase_bw_win = RangeWidget(self._phase_bw_range, self.set_phase_bw, "Phase: Bandwidth", "counter_slider", float)
        self.tab_grid_layout_1.addWidget(self._phase_bw_win, 2,0)
        self._fll_bw_range = Range(0.0, 0.2, 0.001, 0.01, 200)
        self._fll_bw_win = RangeWidget(self._fll_bw_range, self.set_fll_bw, "FLL: Bandwidth", "counter_slider", float)
        self.tab_grid_layout_1.addWidget(self._fll_bw_win, 0,0)
        self._equalizer_gain_range = Range(0.0, 0.2, 0.01, 0.05, 200)
        self._equalizer_gain_win = RangeWidget(self._equalizer_gain_range, self.set_equalizer_gain, "Equalizer: Gain", "counter_slider", float)
        self.tab_grid_layout_1.addWidget(self._equalizer_gain_win, 3,0)
        self._digigain_tool_bar = Qt.QToolBar(self)
        self._digigain_tool_bar.addWidget(Qt.QLabel("digigain"+": "))
        self._digigain_line_edit = Qt.QLineEdit(str(self.digigain))
        self._digigain_tool_bar.addWidget(self._digigain_line_edit)
        self._digigain_line_edit.returnPressed.connect(
        	lambda: self.set_digigain(eng_notation.str_to_num(str(self._digigain_line_edit.text().toAscii()))))
        self.top_grid_layout.addWidget(self._digigain_tool_bar, 0,0)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	1000, #size
        	8000, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.125)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0.disable_legend()
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.tab_layout_2.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_number_sink_0 = qtgui.number_sink(
                gr.sizeof_float,
                0.001,
                qtgui.NUM_GRAPH_NONE,
        	1
        )
        self.qtgui_number_sink_0.set_update_time(0.10)
        self.qtgui_number_sink_0.set_title("")
        
        labels = ["RSSI", "", "", "", "",
                  "", "", "", "", ""]
        units = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"),
                  ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black")]
        factor = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        for i in xrange(1):
            self.qtgui_number_sink_0.set_min(i, -1)
            self.qtgui_number_sink_0.set_max(i, 1)
            self.qtgui_number_sink_0.set_color(i, colors[i][0], colors[i][1])
            if len(labels[i]) == 0:
                self.qtgui_number_sink_0.set_label(i, "Data {0}".format(i))
            else:
                self.qtgui_number_sink_0.set_label(i, labels[i])
            self.qtgui_number_sink_0.set_unit(i, units[i])
            self.qtgui_number_sink_0.set_factor(i, factor[i])
        
        self.qtgui_number_sink_0.enable_autoscale(False)
        self._qtgui_number_sink_0_win = sip.wrapinstance(self.qtgui_number_sink_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_number_sink_0_win, 0,1)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	48e3, #bw
        	"", #name
        	2 #number of inputs
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-100, 0)
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_freq_sink_x_0.disable_legend()
        
        if complex == type(float()):
          self.qtgui_freq_sink_x_0.set_plot_pos_half(not True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(2):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.tab_layout_0.addWidget(self._qtgui_freq_sink_x_0_win)
        self.qtgui_const_sink_x_0 = qtgui.const_sink_c(
        	1024, #size
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_0.set_update_time(0.10)
        self.qtgui_const_sink_x_0.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0.enable_grid(False)
        
        if not True:
          self.qtgui_const_sink_x_0.disable_legend()
        
        labels = ["A", "B", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
                  "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_const_sink_x_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_const_sink_x_0_win, 1,0,1,2)
        self.low_pass_filter_0 = filter.fir_filter_ccf(1, firdes.low_pass(
        	1, samp_rate, 10e3, 2e3, firdes.WIN_HAMMING, 6.76))
        self.lilacsat_vitfilt27_fb_0_0 = lilacsat.vitfilt27_fb()
        self.lilacsat_vitfilt27_fb_0 = lilacsat.vitfilt27_fb()
        self.lilacsat_sync_det_b_0_0 = lilacsat.sync_det_b(0x1ACFFC1D, 116, 1, 1)
        self.lilacsat_sync_det_b_0 = lilacsat.sync_det_b(0x1ACFFC1D, 116, 1, 1)
        self.lilacsat_plan13_cc_0 = lilacsat.plan13_cc(False, tle_line1, tle_line2, lon, lat, alt, 437200000, samp_rate, False, True)
        self.lilacsat_lilacsat1_frame_depack_0 = lilacsat.lilacsat1_frame_depack()
        (self.lilacsat_lilacsat1_frame_depack_0).set_max_output_buffer(4)
        self.lilacsat_codec2_decode_bf_0 = lilacsat.codec2_decode_bf(0)
        self.digital_pfb_clock_sync_xxx_0 = digital.pfb_clock_sync_ccf(sps, timing_loop_bw, (rrc_taps), nfilts, nfilts/2, 0.05, 2)
        self.digital_lms_dd_equalizer_cc_0_0 = digital.lms_dd_equalizer_cc(2, equalizer_gain, 2, variable_constellation_0)
        self.digital_fll_band_edge_cc_0 = digital.fll_band_edge_cc(sps, alpha, 100, fll_bw)
        self.digital_costas_loop_cc_0_0 = digital.costas_loop_cc(phase_bw, 2, False)
        self.blocks_unpack_k_bits_bb_0_0_0_1 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_0_0_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_udp_source_0 = blocks.udp_source(gr.sizeof_gr_complex*1, "127.0.0.1", 7200, 1472, False)
        self.blocks_socket_pdu_0 = blocks.socket_pdu("TCP_CLIENT", "127.0.0.1", "60061", 10000, False)
        self.blocks_rms_xx_0 = blocks.rms_cf(0.0001)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(20, 1, 0)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((10**(digigain/20), ))
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_delay_0 = blocks.delay(gr.sizeof_float*1, 1)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.audio_sink_0 = audio.sink(8000, "", True)
        self.analog_feedforward_agc_cc_0 = analog.feedforward_agc_cc(1024, 2)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.lilacsat_lilacsat1_frame_depack_0, 'out'), (self.blocks_message_debug_0, 'print_pdu'))    
        self.msg_connect((self.lilacsat_lilacsat1_frame_depack_0, 'out'), (self.blocks_socket_pdu_0, 'pdus'))    
        self.msg_connect((self.lilacsat_sync_det_b_0, 'out'), (self.lilacsat_lilacsat1_frame_depack_0, 'in'))    
        self.msg_connect((self.lilacsat_sync_det_b_0_0, 'out'), (self.lilacsat_lilacsat1_frame_depack_0, 'in'))    
        self.connect((self.analog_feedforward_agc_cc_0, 0), (self.digital_fll_band_edge_cc_0, 0))    
        self.connect((self.blocks_complex_to_real_0, 0), (self.blocks_delay_0, 0))    
        self.connect((self.blocks_complex_to_real_0, 0), (self.lilacsat_vitfilt27_fb_0, 0))    
        self.connect((self.blocks_delay_0, 0), (self.lilacsat_vitfilt27_fb_0_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.lilacsat_plan13_cc_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.qtgui_freq_sink_x_0, 0))    
        self.connect((self.blocks_nlog10_ff_0, 0), (self.qtgui_number_sink_0, 0))    
        self.connect((self.blocks_rms_xx_0, 0), (self.blocks_nlog10_ff_0, 0))    
        self.connect((self.blocks_udp_source_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.blocks_unpack_k_bits_bb_0_0_0, 0), (self.lilacsat_sync_det_b_0, 0))    
        self.connect((self.blocks_unpack_k_bits_bb_0_0_0_1, 0), (self.lilacsat_sync_det_b_0_0, 0))    
        self.connect((self.digital_costas_loop_cc_0_0, 0), (self.digital_lms_dd_equalizer_cc_0_0, 0))    
        self.connect((self.digital_fll_band_edge_cc_0, 0), (self.digital_pfb_clock_sync_xxx_0, 0))    
        self.connect((self.digital_fll_band_edge_cc_0, 0), (self.qtgui_freq_sink_x_0, 1))    
        self.connect((self.digital_lms_dd_equalizer_cc_0_0, 0), (self.blocks_complex_to_real_0, 0))    
        self.connect((self.digital_lms_dd_equalizer_cc_0_0, 0), (self.qtgui_const_sink_x_0, 0))    
        self.connect((self.digital_pfb_clock_sync_xxx_0, 0), (self.digital_costas_loop_cc_0_0, 0))    
        self.connect((self.lilacsat_codec2_decode_bf_0, 0), (self.audio_sink_0, 0))    
        self.connect((self.lilacsat_codec2_decode_bf_0, 0), (self.qtgui_time_sink_x_0, 0))    
        self.connect((self.lilacsat_lilacsat1_frame_depack_0, 0), (self.lilacsat_codec2_decode_bf_0, 0))    
        self.connect((self.lilacsat_plan13_cc_0, 0), (self.low_pass_filter_0, 0))    
        self.connect((self.lilacsat_vitfilt27_fb_0, 0), (self.blocks_unpack_k_bits_bb_0_0_0, 0))    
        self.connect((self.lilacsat_vitfilt27_fb_0_0, 0), (self.blocks_unpack_k_bits_bb_0_0_0_1, 0))    
        self.connect((self.low_pass_filter_0, 0), (self.analog_feedforward_agc_cc_0, 0))    
        self.connect((self.low_pass_filter_0, 0), (self.blocks_rms_xx_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "demod_lilacsat1")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.set_rrc_taps(firdes.root_raised_cosine(self.nfilts, self.nfilts, 1.0/float(self.sps), self.alpha, 11*self.sps*self.nfilts))

    def get_nfilts(self):
        return self.nfilts

    def set_nfilts(self, nfilts):
        self.nfilts = nfilts
        self.set_rrc_taps(firdes.root_raised_cosine(self.nfilts, self.nfilts, 1.0/float(self.sps), self.alpha, 11*self.sps*self.nfilts))

    def get_alpha(self):
        return self.alpha

    def set_alpha(self, alpha):
        self.alpha = alpha
        self.set_rrc_taps(firdes.root_raised_cosine(self.nfilts, self.nfilts, 1.0/float(self.sps), self.alpha, 11*self.sps*self.nfilts))

    def get_variable_constellation_0(self):
        return self.variable_constellation_0

    def set_variable_constellation_0(self, variable_constellation_0):
        self.variable_constellation_0 = variable_constellation_0

    def get_timing_loop_bw(self):
        return self.timing_loop_bw

    def set_timing_loop_bw(self, timing_loop_bw):
        self.timing_loop_bw = timing_loop_bw
        self.digital_pfb_clock_sync_xxx_0.set_loop_bandwidth(self.timing_loop_bw)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate, 10e3, 2e3, firdes.WIN_HAMMING, 6.76))

    def get_rrc_taps(self):
        return self.rrc_taps

    def set_rrc_taps(self, rrc_taps):
        self.rrc_taps = rrc_taps
        self.digital_pfb_clock_sync_xxx_0.set_taps((self.rrc_taps))

    def get_phase_bw(self):
        return self.phase_bw

    def set_phase_bw(self, phase_bw):
        self.phase_bw = phase_bw
        self.digital_costas_loop_cc_0_0.set_loop_bandwidth(self.phase_bw)

    def get_fll_bw(self):
        return self.fll_bw

    def set_fll_bw(self, fll_bw):
        self.fll_bw = fll_bw
        self.digital_fll_band_edge_cc_0.set_loop_bandwidth(self.fll_bw)

    def get_equalizer_gain(self):
        return self.equalizer_gain

    def set_equalizer_gain(self, equalizer_gain):
        self.equalizer_gain = equalizer_gain
        self.digital_lms_dd_equalizer_cc_0_0.set_gain(self.equalizer_gain)

    def get_digigain(self):
        return self.digigain

    def set_digigain(self, digigain):
        self.digigain = digigain
        Qt.QMetaObject.invokeMethod(self._digigain_line_edit, "setText", Qt.Q_ARG("QString", eng_notation.num_to_str(self.digigain)))
        self.blocks_multiply_const_vxx_0.set_k((10**(self.digigain/20), ))


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        Qt.QApplication.setGraphicsSystem(gr.prefs().get_string('qtgui','style','raster'))
    qapp = Qt.QApplication(sys.argv)
    tb = demod_lilacsat1()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()
    tb = None  # to clean up Qt widgets
