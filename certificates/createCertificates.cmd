@ECHO OFF

:: this script creates a root ca, a server certificate and
:: their corresponsing keys for use with the ownpush system
::
:: the default hostname is 'ownpush'
:: if you want to change the settings check the *.cnf and
:: the v3.ext files

:: create root ca
c:\OpenSSL-Win64\bin\openssl.exe genrsa -out ca-key.pem 4096
c:\OpenSSL-Win64\bin\openssl.exe req -x509 -new -nodes -key ca-key.pem -sha512 -days 3650 -out ca-root.pem -config root.csr.cnf

:: create self signed certificate
c:\OpenSSL-Win64\bin\openssl.exe req -new -sha512 -nodes -out cert.csr -newkey rsa:4096 -keyout cert-key.pem -reqexts SAN -extensions SAN -config server.csr.cnf
c:\OpenSSL-Win64\bin\openssl.exe x509 -req -in cert.csr -CA ca-root.pem -CAkey ca-key.pem -CAcreateserial -out cert-pub.pem -days 365 -sha512 -extfile v3.ext

:: create combined files

:: ca.pem for use with the clients
md client
copy /b ca-root.pem+ca-key.pem client\ca.pem

:: ownpush certificate chain for use with the server
md server
copy /b cert-pub.pem+ca-root.pem server\ownpush.crt

:: ownpush key for use with the server
copy /b cert-key.pem server\ownpush.key
