#!/bin/bash

CERT=$1

[[ -z "$CERT" ]] && echo "ERROR: type $0 <certificate.cer>" && exit

openssl x509 -in "$CERT"  -text
