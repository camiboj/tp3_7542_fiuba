echo
echo ------- REVOKE INVALID SUBJECT -------
echo

cd ../../../
make -f Makefile_client_server	
cd pruebas/02secuencial-con-valgrind/revoke-invalid-subject
cp ../../../client ../../../server ../revoke-invalid-subject

cp initial-index.txt index.txt
rm client0.out __server_stderr__ __server_stdout__ valgrind_server.out valgrind_client0.out

chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) valgrind

echo 01 - Client0 out
diff client0.out client0_.out
echo 02 - Index
diff index.txt index_.txt
echo 03 - ServerError
diff __server_stderr__ __server_stderr___ 
echo 04 - ServerOut
diff __server_stdout__ __server_stdout___
echo
echo 05 - Server Valgrind
cat valgrind_server.out
echo
echo 06 - Client VAlgrind
cat valgrind_client0.out
