echo
echo ----- CERTIFICATE CREATE -----
cd ../../../
make -f Makefile_client_server	
cd pruebas/02secuencial-con-valgrind/create-certificate
cp ../../../client ../../../server ../create-certificate

rm __server_stdout__ __server_stderr__ "Federico Manuel Gomez Peter.cert" index.txt
cp initial-index.txt index.txt

chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) valgrind


echo 01 - Ceritificado de Fede
diff "Federico Manuel Gomez Peter.cert" "Federico Manuel Gomez Peter_.cert"
echo 02 - Certificate Client0
diff "client0.out" "client0_.out"
echo 03 - Index
diff index.txt index_.txt
echo 04 - ServerError
diff __server_stderr__ __server_stderr___ 
echo 05 - ServerOut
diff __server_stdout__ __server_stdout___

echo
echo 06 - Client Valgrind
cat valgrind_client0.out

echo
echo 07 - Server Valgrind
cat valgrind_server.out
