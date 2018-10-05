openssl req -new > new.cert.csr
openssl rsa -in privkey.pem -out new.cert.key
openssl x509 -in new.cert.csr -out new.cert.cert -req -signkey new.cert.key -days 365
cp new.cert.cert server.crt
cp new.cert.key server.key