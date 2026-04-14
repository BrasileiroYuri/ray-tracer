# Ray Tracer Experimental (RT-Proj)

Este projeto consiste em um motor de renderização via Ray Tracing desenvolvido para a disciplina de Computação Gráfica. O sistema simula a trajetória de raios de luz para gerar imagens sintéticas a partir de descrições matemáticas de cenas.

## Mudanças e Implementações (Projeto 03)
* **Sistema de Coordenadas:** Adoção da Regra da Mão Esquerda (Left-Hand Rule) para alinhamento intuitivo com o eixo Z positivo do XML.
* **Estabilidade Numérica:** Reformulação do cálculo do discriminante da esfera para suportar interseções em grandes distâncias, eliminando erros de precisão de ponto flutuante (cancelamento catastrófico).
* **Integração XML:** Implementação de suporte para as tags `<integrator>` e `<object type="sphere">`.
* **Correção de Projeção:** Implementação do cálculo de Aspect Ratio na `ScreenWindow` para evitar o achatamento de objetos e inversão do eixo vertical (Y) na geração de raios para correta orientação da cena.

## Componentes Fundamentais
* **App:** Gerencia o ciclo de vida da renderização, o parsing do XML e o loop principal de pixels.
* **Camera:** Classe base que define a base ortonormal (u, v, w). Subdividida em `Perspective` (raios convergentes) e `Orthographic` (raios paralelos).
* **Primitive (Sphere):** Implementa a geometria e os algoritmos de interseção estável raio-objeto.
* **Film:** Buffer de memória para armazenamento de pixels e exportação para formatos PNG e PPM.
* **ParamSet:** Estrutura para armazenamento e conversão de parâmetros extraídos do XML para tipos nativos do C++.

## Compilação e Execução
O projeto utiliza CMake para o gerenciamento do build.

1. **Configurar o build:**
   ```bash

   cmake -S . -B build

   ```
2. **Compilar:**
    ```bash

   cmake --build build 

    ```
3. **Executar:**
    ```bash

   ./build/rt3 <--options> scenes/arquivo.xml   
   
    ```

## Autores
* Marcos Antônio Fontes Leite
* Yuri Maximiliano Brasileiro