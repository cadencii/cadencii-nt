#!/usr/bin/env ruby

require "rexml/parsers/sax2parser"
require "rexml/sax2listener"

class Listener
  include REXML::SAX2Listener

  def initialize output_handle
    @handle = output_handle
  end

  def start_element uri, localname, qname, attributes
    @handle.puts("startElement\t#{qname}")
  end

  def end_element uri, localname, qname
    @handle.puts("endElement\t#{qname}")
  end

  def characters ch
    @handle.puts("characters\t#{ch}")
  end
end

input = ARGV[0]
output = open(ARGV[1], "w")

source = REXML::SourceFactory::create_from(open(input).read)
parser = REXML::Parsers::SAX2Parser::new(source)
parser.listen(Listener::new(output))
parser.parse
