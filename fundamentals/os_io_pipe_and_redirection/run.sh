#!/usr/bin/env bash

cd bin
# Redirect stdin to csv_reader with books.csv,
# csv_reader stdout is connected to csv_to_json stdin through pipe,
# then stdout is redirected to books.json
# (./build/csv_reader | ./build/csv_to_json) < books.csv > books.json
(./csv_reader | ./csv_to_json) < books.csv > books.json

