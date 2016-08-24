# T1-Seguranca-PortScanner
Trabalho feito para a disciplina CI301 - Introdução à Segurança Computacional, mestrada pelo Professor André Grégio, da  Universidade Federal do Paraná
===========================================
# Compilação
Para compilar:  gcc portscanner.c -o portscanner

Entrada esperada: ./portscanner <range de IP ou um IP> <range de porta ou uma porta>
Exemplo: 
  ./port_scanner 10.254.223.39 22
  ./port_scanner 10.254.223.39-55 22-30
  
Saída:

Varredura iniciada em Wed Aug 24 21:28:51 2016

IP: 10.254.223.39
Portas: 22

---------

10.254.223.39	 22	 SSH-2.0-OpenSSH_7.2p2 Ubuntu-4ubuntu1

