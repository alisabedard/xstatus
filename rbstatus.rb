#!/usr/bin/env ruby

class Status
	$load="/proc/loadavg"
	$temp="/sys/devices/virtual/thermal/thermal_zone0/temp"
	$bat="/sys/class/power_supply/BAT0/capacity"
	def self.get_load
		open($load).read.split(' ')[0]
	end
	def self.get_temp
		open($temp).read.split('\n')[0].to_i/1000 
	end
	def self.get_bat
		open($bat).read.rstrip 
	end
	def self.format
		"#{self.get_load} | #{self.get_temp}C | #{self.get_bat}%\n"
	end
	def self.format
		"#{self.get_load} | #{self.get_temp}C\n"
	end
end

# Main
if __FILE__ == $0
	$tmp = "/tmp/xstatus"
	# Traps to clean up $tmp and exit quietly
	Signal.trap("INT") do exit end
	Signal.trap("EXIT") do File.delete($tmp) end
	loop do
		# Block forces re-opening on loop iteration
		open($tmp, "w") { |f| f.puts Status.format }
		sleep(5)
	end
end

