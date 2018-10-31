#!/usr/bin/env bash

#############
# this script creates a root ca, a server certificate and
# their corresponsing keys for use with the ownpush system
#
# the default hostname is 'ownpush'
# if you want to change the settings check the *.cnf and
# the v3.ext files
#############

# create root ca
openssl genrsa -out ca-key.pem 4096
openssl req -x509 -new -nodes -key ca-key.pem -sha512 -days 3650 -out ca-root.pem -config <( cat root.csr.cnf )

# create self signed certificate
openssl req -new -sha512 -nodes -out cert.csr -newkey rsa:4096 -keyout cert-key.pem -reqexts SAN -extensions SAN -config <( cat server.csr.cnf )
openssl x509 -req -in cert.csr -CA ca-root.pem -CAkey ca-key.pem -CAcreateserial -out cert-pub.pem -days 365 -sha512 -extfile v3.ext

#create combined files

## ca.pem for use with the clients
mkdir client
cat ca-root.pem ca-key.pem >> client/ca.pem

## ownpush certificate chain for use with the server
mkdir server
cat cert-pub.pem ca-root.pem >> server/ownpush.crt

## ownpush key for use with the server
cat cert-key.pem >> server/ownpush.key
