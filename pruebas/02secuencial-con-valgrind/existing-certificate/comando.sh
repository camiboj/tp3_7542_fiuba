echo
echo ----- EXISTING CERTIFICATE -----
cd ../../../
make -f Makefile_client_server	
cd pruebas/02secuencial-con-valgrind/existing-certificate
cp ../../../client ../../../server ../existing-certificate

rm __server_stdout__ __server_stderr__ client0.out client1.out "Federico Manuel Gomez Peter.cert"
cp initial-index.txt index.txt

chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) valgrind


echo 01 - Ceritificado de Fede
diff "Federico Manuel Gomez Peter.cert" "Federico Manuel Gomez Peter_.cert"
echo 02 - out Client0
diff "client0.out" "client0_.out"
echo 03 - out Client1
diff "client1.out" "client1_.out"
echo 04 - Index
diff index.txt index_.txt
echo 05 - ServerError
diff __server_stderr__ __server_stderr___ 
echo 06 - ServerOut
diff __server_stdout__ __server_stdout___

echo
echo 07 - Client Valgrind 
cat valgrind_client0.out

echo
echo 08 - Server Valgrind 
cat valgrind_server.out
