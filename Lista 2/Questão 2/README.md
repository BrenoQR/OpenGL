Matrizes de Transformação

Matriz Identidade I:
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

Translação T(x,y,z):
[ 1 0 0 x ]
[ 0 1 0 y ]
[ 0 0 1 z ]
[ 0 0 0 1 ]

Logo:
T1 = T2 = T(0.5, 0.5, 0) =
[ 1 0 0 0.5 ]
[ 0 1 0 0.5 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

Rotação em torno de Z, θ = 45° (π/4):
R =
[ √2/2 -√2/2 0 0 ]
[ √2/2 √2/2 0 0 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

Evolução da pilha (assumindo pilha inicial com [I])

Passo 0:
Pilha = [ I ]

Passo 1: glPushMatrix()
Pilha = [ I, I ]

Passo 2: glTranslatef(0.5, 0.5, 0)
Pilha = [ I, T1 ]

Passo 3: glRectf(...)
Desenha quadrado transformado por T1.

Passo 4: glPopMatrix()
Pilha = [ I ]

Passo 5: glColor3f(...) (não altera pilha)

Passo 6: glPushMatrix()
Pilha = [ I, I ]

Passo 7: glTranslatef(0.5, 0.5, 0)
Pilha = [ I, T2 ]

Passo 8: glRotatef(45, 0, 0, 1)
Topo = T2 · R
Pilha = [ I, T2·R ]

Passo 9: glRectf(...)
Desenha quadrado transformado por T2·R.

Passo 10: glPopMatrix()
Pilha = [ I ]

Matrizes finais (escritas explicitamente)

T1 = T2 =
[ 1 0 0 0.5 ]
[ 0 1 0 0.5 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

R =
[ √2/2 -√2/2 0 0 ]
[ √2/2 √2/2 0 0 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

Produto T2 · R (matriz resultante usada para desenhar o segundo quadrado):
T2·R =
[ √2/2 -√2/2 0 0.5 ]
[ √2/2 √2/2 0 0.5 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]
