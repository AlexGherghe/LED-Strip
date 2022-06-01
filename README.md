# LED-Strip

## Descriere
O banda LED controlabila prin aplicatie ce poate reactiona la muzica.

Proiectul a fost realizat folosind un NodeMCU, o banda LED RGB alimentata la 12V si un modul microfon MAX4466. Pentru a controla banda LED am folosit 3 MOSFET-uri LU3715Z. Banda proceseaza semnalul primit de la microfon si, cu ajutorul transformatei Fourier rapide, asociaza o culoare fiecarei plaje de frecvente. In plus, placuta NodeMCU se conecteaza la reteaua WI-FI si poate primi pachete UDP, prin care se poate porni/opri modul pentru muzica si se pot schimba culoarea si intensitatea benzii LED.

## Fotografie
![Fotografie proiect](/fotografie.png)

## Schema electrica
![Schema electrica](/circuit.png)

## Demo 
https://youtu.be/xjvuOrYAR-w
