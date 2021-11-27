# LVP - Laboratório Virtual de Petrofísica 

## Sobre

O software LVP oferece ao usuário diversas funcionalidades de simulação e determinação de propriedades petrofísicas através da análise de imagens de meios porosos. O propósito inicial no desenvolvimento deste software foi o de facilitar, através de uma interface gráfica amigável e avançada, a utilização dos modelos e algoritmos desenvolvidos como parte de um trabalho de mestrado, e posteriormente de doutorado, realizado na UENF/LENEP. Devido ao fato dos trabalhos terem como base a biblioteca para análise de imagens de meios porosos lib_ldsc, diversas outras funcionalidades implementadas nesta biblioteca foram disponibilizadas no LVP.

Dentre as funcionalidades podemos destacar:

 - Aplicação de filtros em imagens 2D e 3D, incluindo: conectividade, imagem de distância ao fundo, inversão da imagem, passa-baixa e passa-alta;

 - Aplicação dos filtros morfológicos: fechamento, abertura, dilatação e erosão;

 - Visualização plano a plano de imagens tridimensionais permitindo escolher o eixo de visualização;

 - Visualização de imagens tridimensionais e bidimensionais;

 - Determinação de correlações frequencial e espacial;

 - Determinação da distribuição de tamanho de poros e sólidos;

 - Calculo da permeabilidade intrínseca;

 - Calculo da permeabilidade relativa;

 - Visualização das curvas de permeabilidade relativa;

 - Visualização dos gráficos de correlações;

 - Calculo da porosidade;

 - Rotação de imagens tridimensionais;

 - Corte de imagens tridimensionais;

 - Determinação das configurações de equilíbrio;

 - Reconstruções tridimensionais através dos métodos esferas sobrepostas, gaussiana truncada e gaussiana truncada revisada;

 - Segmentação de poros e gargantas, através de operações morfológicas;

 - Criação de redes de percolação representativas ao meio poroso;

 - Calculo da permeabilidade intrínseca nas redes de percolação.

## Rodando o projeto

> É que recomendado que, em um primeiro momento, você utilize o `QtCreator` para a configuração e execução do LVP.

Para compilar o LVP você irá precisar das seguintes bibliotecas:
 - Qt 5.5 - http://www.qt.io/
 - Lib_LDSC - https://github.com/ldsc/lib_ldsc
 - QWT 6.1.2 - https://sourceforge.net/projects/qwt/files/qwt/6.1.2/
 - LVP - https://github.com/ldsc/lvp

> Certifique que o seu compilador suporta parâmetros da biblioteca OpemMP e que tanto a biblioteca OpenMP quanto as  bibliotecas OpenGl e Glu, estejam instaladas no seu ambiente de desenvolvimento.

### Qt
 - Baixe a versão 5.5 e siga os procedimentos de instalação informados no site;
 - Sugerimos a utilização do 'Qt Maintenance Toll' para gerenciar os pacotes da Qt;
 - Sugerimos ainda a utilização da IDE QtCreator para abrir os projetos e executar a compilação;
 - No Windows, utilize o compilador MinGW 4.9.2 (maior compatibilidade) o qual pode ser instalado a partir do 'Qt Maintenance Toll'.
 
### Lib_LDSC
 - Baixe a biblioteca no link informado (você pode baixar toda a biblioteca em um arquivo .zip ou baixar uma cópia do repositório Git);
 - Importe o projeto para o QtCreator, a partir do arquivo CMakeLists.txt;
 - Na aba do projeto, ajuste os parâmetros de compilação e instalação da biblioteca;
 - Certifique que o Cmake esteja instalado em seu ambiente de desenvolvimento e devidamente configurado no QtCreator;
 - Execute o comando cmake, para gerar o arquivo makefile e o comando make para compilar a biblioteca (estes comandos podem ser executados a partir do QtCreator).

### QWT
 - Na página informada, baixe o aquivo compactado (.zip para Windows e .tar.bz2 para Linux);
 - Descompacte o arquivo em seu diretório de desenvolvimento;
 - Edite o arquivo qwtconfig.pri ajustando os seguintes parâmetros:

 ```bash
QWT_INSTALL_PREFIX # Informe o diretório de instalação desejado;
QWT_CONFIG += QwtDll # descomente esta linha para que a biblioteca seja dinâmica
QWT_CONFIG += QwtPlot # descomente esta linha para utilizar o widget QwtPlot
QWT_CONFIG += QwtWidgets # descomente esta linha para utilizar os demais widgets da QWT
 ```

 - A partir do arquivo de projeto `qwt.pro` abra a biblioteca no QtCreator e, na aba do projeto, ajuste os parâmetros de compilação e instalação da biblioteca;
 - Execute o comando Qmake para gerar o arquivo makefile e o comando make para compilar a biblioteca.

### LVP
 - Baixe o projeto na pagina informada e abra no QtCreator, a partir do arquivo do projeto `lvp.pro`;
 - No QtCreator, na aba do projeto, ajuste os parâmetros de compilação e instalação do software;
 - Edite o arquivo `src/src.pro` e ajuste os parâmetros INCLUDEPATH e LIBS, de acordo com a localização das biblioteca e includes do seu ambiente de desenvolvimento;
 - Execute o comando Qmake para gerar o arquivo makefile e o comando make para compilar os software;
 - Execute o software a partir da própria IDE ou diretamente a partir do aquivo executável gerado no diretório `/bin`:
```bash
$ bin/lvp
```
 - Para executar diretamente, talvez seja necessório copiar, para o diretório de execução, as bibliotecas dinâmicas utilizadas pelo software.

 > Verifique se a `lib_ldsc` está associada à variável de ambiente `LD_LIBRARY_PATH`. Isso garante que o `LVP` possa ser executado de forma correta pelo terminal.

### Execução pelo terminal

Após a execução desses passos é possivel criar um link simbólico no diretório `/usr/bin` para que dessa forma o `LVP` possa ser executado diretamente pelo terminal. Para isso execute:

```bash
$ install_optional/install /caminho/completo/do/lvp
$ lvp
```
