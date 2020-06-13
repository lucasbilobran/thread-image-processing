# Laboratório de CES-33: Pré-processamento de Imagens utilizando Threads

Alunos:
- Gianluigi Dal Toso [@gitoso](https://github.com/gitoso)
- Lucas Alberto Bilobran Lema [@lucasbilobran](https://github.com/lucasbilobran)

##  1. <a name='TabeladeContedos'></a>Tabela de Conteúdos

* 1. [Tabela de Conteúdos](#TabeladeContedos)
* 2. [Contextualização](#Contextualizao)
* 3. [Tarefas propostas](#Tarefaspropostas)
* 4. [Compilação & Execução](#CompilaoExecuo)
* 5. [Execução dos benchmarks](#Execuodosbenchmarks)
* 6. [Referências](#Referncias)

##  2. <a name='Contextualizao'></a>Contextualização

Com o avanço dos modelos de aprendizado de máquina,
especialmente os baseados em redes neurais profundas,
diversas aplicações de Inteligência Artificial tem surgido,
especialmente no que tange a análise de imagens, com
aplicações que vão desde a categorização e descrição de
imagens até o diagnóstico médico em imagens de exames.
No entanto, o trabalho computacional de um algoritmo de
Inteligência Artificial de reconhecimento de imagens ainda
é inviável para se conseguir trabalhar com um sistema de
tempo real com imagens em alta definição.
Os sistemas que trabalham em tempo real, como o
reconhecimento de placas de carro através de imagens de
câmeras, costumam redimensionar e comprimir a imagem
antes de transmiti-la ao algoritmo de IA. Além disso, rara-
mente a imagem “crua” é suficiente para ser utilizada pelos
algoritmos de reconhecimento. O tradicional é realizar o
pré-processamento dessa imagem, aplicando-se filtros que
a tornarão mais adequada para o algoritmo. Esses filtros
dependem muito da tarefa em questão, e podem incluir
a remoção de cores, binarização dos pixels, remoção de
ruı́dos, dentre diversos outros filtros. As referências [1] e [2]
ilustram procedimentos de pré-processamento de imagens
para uma aplicação em um modelo de IA para visão
computacional.
Dessa forma, acelerar a etapa de pré-processamento
de imagens pode viabilizar uma aplicação de IA em um
sistema de tempo real. Ademais, para sistemas de análise
de imagens que já conseguem trabalhar em tempo real,
reduzir o tempo de pré-processamento pode viabilizar que
se reduza o nı́vel de compressão e redimensionamento
da imagem, podendo-se então trabalhar com imagens de
qualidade e resolução superior.

##  3. <a name='Tarefaspropostas'></a>Tarefas propostas

A tarefa proposta para o laboratório irá consistir de montar duas versões de um pipeline de threads cada
qual realizando uma das tarefas de pré-processamento.
Existirá uma thread retirando imagens de um banco de
dados de imagens, e repassando-as para o pipeline de pré-
processamento, como se fosse uma câmera transmitindo
dados. A comunicação entre as threads se dará por meio
de um buffer compartilhado e será portando uma comu-
nicação na forma produtor-consumidor. 

A primeira versão
do pipeline consistirá de uma thread realizando cada ati-
vidade de pré-processamento. A segunda versão consistira
de um pipeline onde múltiplas threads realizam cada ati-
vidade de pré-processamento, neste caso aproveitando-se
do paralelismo para acelerar a aplicação, porém existindo
mais dificuldades para gerir o recurso compartilhado.

##  4. <a name='CompilaoExecuo'></a>Compilação & Execução
Nesta seção será descrita como realizar a compilação em sistemas Linux para a distribuição Ubuntu. Para outras distribuições é possível realizar procedimento análogo.

Antes de iniciar a compilação, deve-se instalar a biblioteca __GraphicsMagick__, que é uma dependência do projeto.

```
sudo apt install libgraphicsmagick1-dev
```

Após instalada a dependência, basta compilar o projeto utilizando a ferramenta __Make__:

```
cd build/
make
```

Estando o projeto compilado, basta executar os binários que foram criados pela compilação na pasta `build/` e passar como argumento os arquivos de imagem que deseja-se processar.

Por exemplo:

```
./SimpleThread ../images/dummyset/*.jpg
```

Caso seja desejável, é possível acompanhar as imagens resultantes da execução do programa monitorando-se a pasta `output/`.

##  5. <a name='Execuodosbenchmarks'></a>Execução dos benchmarks
Os scripts de benchmark por padrão limpam os arquivos produto de qualquer execução anterior, realizam uma nova compilação limpa e em seguida executam uma bateria de testes para avaliar o laboratório implementado. 

Existem duas variações do benchmark, uma mais enxuta (versão Lite) e outra mais completa. Mesmo a versão mais leve já pode ser bastante custosa para o sistema. Para executar os benchmarks:

```
cd benchmark/
./benchmark-lite.sh
```

##  6. <a name='Referncias'></a>Referências
[1] Prince Canuma “Image Pre-processing” em https://towardsdatascience.com/ [Online] Disponı́vel em: https://towardsdatascience.com/image-pre-processing-c1aec0be3edf

[2] Krig, Scott. “Computer vision metrics.” Capı́tulo 2:Image Pre-Processing, 2016. [Online] Disponı́vel em: https://www.embedded-vision.com/sites/default/files/apress/computervision
