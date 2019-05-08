## No en las correcciones:
- Sobrecargar el mismo send en Protocolo. 

- trow exceptions (sobre todo en el Socket)


## si estan
- pasar por referencia en Certificate (?)
- no pasar punteros en protocol
- agregar delete al constructor x copia.
- "Lo que te está sucediendo es que como el canal está cerrado, recibís siempre cero en ambos lados. Cuando ejecutas el
comando new en el cliente, el recibir cero implica un error, y en el comando revoke cierra "ordenadamente" ante el
"exito" de la revocación."


## dudas