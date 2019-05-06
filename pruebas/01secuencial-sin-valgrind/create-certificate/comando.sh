cd ../../../
make -f Makefile_client_server	
cd pruebas/01secuencial-sin-valgrind/create-certificate
cp ../../../client ../../../server ../create-certificate

rm __server_stdout__ __server_stderr__ "Federico Manuel Gomez Peter.cert" index.txt
cp initial-index.txt index.txt

chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) no-valgrind

echo 01 - Certificado de Fede
diff "Federico Manuel Gomez Peter.cert" "Federico Manuel Gomez Peter_.cert"
echo 02 - Index
diff index.txt index_.txt
echo 03 - ServerError
diff __server_stderr__ __server_stderr___ 
echo 04 - ServerOut
diff __server_stdout__ __server_stdout___
echo FIN
