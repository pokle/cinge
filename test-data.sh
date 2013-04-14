#!/usr/bin/env bash

ESURI='http://10.0.0.10:9200'

curl -XDELETE $ESURI/lump

curl -D - -s -XPOST $ESURI/_bulk --data-binary @test-data.json
echo