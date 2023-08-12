Durante o curso de "Microcontroladores e Microprocessadores", tive a 
oportunidade de explorar os fundamentos teóricos e práticos relacionados ao 
desenvolvimento de circuitos eletrônicos e ao uso do Arduino como uma plataforma 
versátil para implementar soluções inovadoras. Com base nesse conhecimento 
adquirido, foi proposto um projeto que desafiou nossa criatividade e habilidades 
técnicas, permitindo a aplicação prática dos elementos estudados ao longo do curso.
Nesse contexto, o trabalho desenvolvido foi uma oportunidade para integrar os 
conceitos teóricos com a prática, colocando em prática nossas habilidades de 
programação, circuitos eletrônicos e manipulação do Arduino. Nosso objetivo foi 
criar um projeto que trouxesse benefícios reais e utilidade no cotidiano, aplicando 
os conhecimentos de microcontroladores e microprocessadores de forma concreta
O código desenvolvido foi dividido em cinco partes, trabalhando em 
conjunto de forma cíclica para executar as funcionalidades propostas. Essas 
partes são:
1. Código para o sensor de umidade e temperatura do ar: responsável pela 
leitura dos valores de umidade e temperatura utilizando o sensor.
2. Código para a leitura e salvamento dos dados no cartão SD: realiza a 
leitura das informações medidas e as salva em um arquivo chamado 
"dados.txt" no cartão SD. Caso o arquivo não exista, ele é criado.
Exemplo do arquivo gerado: 
1
3. Código para o acionamento da mini bomba e irrigação automática: 
controla a bomba de irrigação para realizar a rega automática das plantas.
4. Código do display: exibe as informações medidas, como a temperatura e 
umidade atuais do ar, no display. Além disso, cria uma string contendo a 
data e hora para serem salvos no cartão SD.
5. Código dos LEDs: implementa uma sequência de utilização dos LEDs. 
Esse código está localizado ao final do setup, mas pode ser acionado e 
modificado conforme a necessidade do usuário.
Durante a execução do código, a ativação da tela do LED ocorre 
simultaneamente com a leitura do sensor, capturando os valores de 
temperatura e umidade do ar. Em seguida, o cartão SD é inicializado e o 
arquivo "dados.txt" é aberto (ou criado, se necessário). As informações 
medidas são então apresentadas no display, exibindo a temperatura e 
umidade atual do ar.
O código também verifica se é hora de acionar a bomba de irrigação, pois 
esse ciclo precisa ocorrer a cada intervalo de 12 horas, contadas a partir da 
conexão do Arduino à fonte de energia.

Lembrando que o código dos LEDs pode ser acionado e alterado conforme a 
vontade do usuário, proporcionando diferentes sequências e efeitos 
luminosos.
