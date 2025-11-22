# Guia de Compilação e Utilização - Visualizador OBJ com OpenGL

## Compilação do Executável

Para compilar o programa no Linux, execute:

```bash
gcc main.c -o visualizador -lGL -lGLU -lglut -lm
```

## Modelos Utilizados

O programa foi testado com sucesso com os seguintes modelos:
- **bunny.obj** (Coelho Stanford)
- **dragon.obj** (Dragão)
- **buddha.obj** (Estátua de Buda)

Todos os modelos devem estar no **mesmo diretório** do executável. Os arquivos devem estar em formato **.obj** padrão (wavefront). Todos os modelos estão disponíveis neste site:
https://casual-effects.com/data/index.html


## Dependências Necessárias

### Bibliotecas OpenGL (Linux Debian/Ubuntu):
```bash
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

### Arquivos Obrigatórios:
- main.c (código-fonte)
- `bunny.obj`, `dragon.obj`, `buddha.obj` (modelos 3D)

## Instruções de Uso

Execute o programa:
```bash
./visualizador
```

**Controles disponíveis:**
- **Botão esquerdo do mouse**: rotacionar o objeto (arrastar)
- **Scroll do mouse**: zoom (aumentar/diminuir)
- **Tecla '1'**: exibir Dragão
- **Tecla '2'**: exibir Coelho
- **Tecla '3'**: exibir Buda
- **ESC**: sair do programa

## Informações Relevantes

- O programa normaliza automaticamente os modelos (centraliza e escala) para visualização adequada
- Implementa **flat shading** com cálculo de normais por face
- Suporta iluminação básica com uma luz direcional
- O zoom está limitado entre 0.5 e 10.0 unidades para evitar câmera invertida
- A rotação pode ser feita em qualquer direção sem travamentos