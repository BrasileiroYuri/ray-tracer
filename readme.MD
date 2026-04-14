# 📸 Ray Tracer Experimental (RT-Proj)

Este repositório contém o desenvolvimento de um motor de renderização via **Ray Tracing**, desenvolvido para a disciplina de **Computação Gráfica**. O projeto foca na síntese de imagens 3D através da simulação matemática de trajetórias de raios de luz.

## 🛠️ O Projeto (Sprint: Geometria e Câmeras)

Atualmente, o motor é capaz de processar cenas complexas definidas via XML, suportando diferentes modelos de projeção e a renderização de primitivas esféricas com alta precisão matemática.

### 🚀 Funcionalidades Principais
* **Modelos de Câmera:**
    * **Perspectiva:** Projeção realista com controle de campo de visão (*fovy*) e correção de *Aspect Ratio*.
    * **Ortográfica:** Projeção paralela para visualizações técnicas sem distorção de profundidade.
    * **Look-At:** Posicionamento dinâmico utilizando a **Regra da Mão Esquerda**.
* **Geometria de Esferas:** Algoritmo de interseção raio-esfera otimizado para lidar com múltiplas primitivas na mesma cena.
* **Gestão de Cena (Parser):** Sistema robusto que mapeia tags XML diretamente para objetos C++ via `ParamSet`.
* **Backgrounds:** Suporte a cores sólidas e degradês interpolados (4-colors).

---

## 🏗️ Arquitetura do Sistema

O motor foi construído seguindo princípios de POO (Programação Orientada a Objetos) para permitir fácil expansão:

| Componente | Responsabilidade |
| :--- | :--- |
| `App` | Orquestrador principal da renderização e gerenciamento de estado. |
| `Camera` | Classe base abstrata para geração de raios (Perspective/Orthographic). |
| `Primitive` | Abstração para objetos da cena (atualmente implementado em `Sphere`). |
| `ParamSet` | Estrutura de dados para armazenamento e conversão de atributos do XML. |
| `Film` | Buffer de memória para armazenamento e escrita dos pixels (PNG/PPM). |

---

## 📐 Detalhes de Implementação

### Regra da Mão Esquerda (Left-Hand Rule)
Para alinhar o motor com as definições comuns de arquivos de cena, adotamos o sistema de coordenadas de mão esquerda:
* **+X:** Direita
* **+Y:** Cima
* **+Z:** Frente (Entrando na cena)


### Integrador Flat
Nesta etapa, utilizamos o **Flat Integrator**, que atribui a cor intrínseca do objeto ao pixel se houver interseção, ignorando temporariamente modelos de iluminação para validar a precisão do sistema de visibilidade.

---

## 🔧 Como Compilar e Rodar

1.  **Requisitos:** GCC (C++17) e GNU Make.
2.  **Compilação:**
    ```bash
    make
    ```
3.  **Execução:**
    ```bash
    ./rt3 <--options> scenes/minha_cena.xml
    ```

---
## 👥 Autores
* **Marcos Antônio Fontes Leite**
* **Yuri Maximiliano Brasileiro**

---