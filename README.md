# Ray Tracer Experimental (RT-Proj)

Este projeto consiste em um motor de renderização via Ray Tracing desenvolvido para a disciplina de Computação Gráfica. O sistema simula a trajetória de raios de luz para gerar imagens sintéticas a partir de descrições matemáticas de cenas.

## Mudanças e Implementações (Projeto 03)

* **Sistema de Coordenadas:** Adoção da Regra da Mão Esquerda (Left-Hand Rule) para alinhamento intuitivo com o eixo Z positivo do XML.
* **Estabilidade Numérica:** Reformulação do cálculo do discriminante da esfera para suportar interseções em grandes distâncias, eliminando erros de precisão de ponto flutuante (cancelamento catastrófico).
* **Integração XML:** Implementação de suporte para as tags `<integrator>` e `<object type="sphere">`.
* **Correção de Projeção:** Inversão do eixo vertical (Y) na geração de raios para correta orientação da cena.

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

   ./build/rt3 [--options] scenes/arquivo.xml

    ```

## Crédito Extra: Esferas Parciais

Implementamos o suporte a **Esferas Parciais**, permitindo que a geometria da esfera seja truncada tanto verticalmente quanto lateralmente através de parametrização por coordenadas esféricas.

### Funcionalidades Implementadas

* **Corte Vertical (Eixo Z):** Através dos parâmetros `z_min` e `z_max`, a esfera pode ser cortada em planos horizontais, permitindo a criação de hemisférios e calotas esféricas.
* **Corte Lateral (Azimute $\phi$):** Implementação do parâmetro `phi_max`, que limita a varredura da esfera no plano horizontal ($0$ a $360^\circ$), possibilitando a renderização de fatias ou "gomos".
* **Interseção Robusta:** O algoritmo de interseção foi ajustado para testar ambas as raízes da equação quadrática ($t_1$ e $t_2$). Isso garante que, se a face frontal for removida pelo corte, o raio atinja a parede interna da esfera, mantendo a integridade visual da forma.
* **Parser XML Dinâmico:** O sistema de leitura foi expandido para reconhecer e processar automaticamente os novos atributos diretamente do arquivo de cena.

### Exemplo de Configuração XML

Para utilizar as esferas parciais, basta adicionar os novos atributos opcionais na tag de objeto:

```xml
<object type="sphere" radius="1.0" center="0 0 5"
        z_min="-1.0" z_max="1.0" phi_max="90" />

```

## Autores

* Yuri Maximiliano Brasileiro
* Marcos Antônio Fontes Leite

