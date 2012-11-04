#!/bin/bash

function which_ruby {
    local ruby=$(which ruby 2>/dev/null)
    if [ -z "$ruby" -a -e "${HOME}/.rvm/rubies/default/bin/ruby" ]; then
        ruby=${HOME}/.rvm/rubies/default/bin/ruby
    fi

    if [ -z "$ruby" ]; then
        echo "ruby not found"
        exit 1
    fi

    echo $ruby
}

