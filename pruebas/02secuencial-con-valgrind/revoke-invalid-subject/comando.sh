echo
echo ----- REVOKE INVALID SUBJECT -----
cd ../../../
make -f Makefile_client_server	
cd pruebas/02secuencial-con-valgrind/revoke-invalid-subject
cp ../../../client ../../../server ../revoke-invalid-subject

rm client0.out index.txt __server_stderr__ __server_stdout__
cp initial-index.txt index.txt

chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) valgrind

echo 01 - Certificate Client0
diff "client0.out" "client0_.out"
echo 02 - Index
diff index.txt index_.txt
echo 03 - ServerError
diff __server_stderr__ __server_stderr___ 
echo 04 - ServerOut
diff __server_stdout__ __server_stdout___


echo
echo 05 - Client Valgrind
cat valgrind_client0.out

echo
echo 06 - Server Valgrind
cat valgrind_server.out
