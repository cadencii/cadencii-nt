#!/usr/bin/env ruby
require "rexml/document"

inputFile = ARGV[0]
outputFile = ARGV[1]

result = REXML::Document.new(open(inputFile).read)

result.root.each_element("logRotator") { |logRotator|
  result.root.delete_element(logRotator)
}
result.root.each_element("scm/branches/hudson.plugins.git.BranchSpec/name") { |name|
  name.text = "master"
}

result.write(open(outputFile, "w"))

