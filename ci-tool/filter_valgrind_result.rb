#!/usr/bin/env ruby
require "rexml/document"

file = ARGV[0]

result = REXML::Document.new(open(file).read)

result.root.each_element("error") { |error|
  error.each_element("stack/frame") { |frame|
    if frame.get_text("file") == "TestAssert.h" then
      result.root.delete_element(error)
    end
    break
  }
}

result.write(open(file, "w"))

