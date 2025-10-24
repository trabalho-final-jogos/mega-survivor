[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/R6H2vFsw)
# TP3: Super Mario Bros
**Autor:** Filipe Barros Vitorino

Instruções: [https://lucasnfe.github.io/dcc192-2025-2/avaliacoes/tp3-smb]([https://lucasnfe.github.io/dcc192-2025-2/avaliacoes/tp3-smb](https://lucasnfe.github.io/dcc192-2025-2/avaliacoes/tp3-smb))


# Novas Funcionalidades Implementadas 

Este documento descreve as funcionalidades da parte de customização.

## Mario: Crescimento, Encolhimento e Invulnerabilidade

O Mario agora possui a habilidade de crescer ao coletar um cogumelo e encolher ao ser atingido por um inimigo enquanto está grande, com um período de invulnerabilidade temporária.

### 1. Crescimento (`Mario::Grow()`)

* **Gatilho:** Chamado quando Mario colide (horizontal ou verticalmente) com um `Actor` na camada `ColliderLayer::PowerUp` (atualmente, o `Mushroom`).
* **Ação:**
    * Verifica se Mario já está grande (`mIsBig`). Se sim, a função retorna.
    * Define o estado interno `mIsBig = true`.
    * Chama `mAnimator->LoadSheet()` para carregar a nova folha de sprites (`SuperMario.png`) e os dados correspondentes (`SuperMario.json`), limpando os dados e animações anteriores.
    * Chama `mAnimator->AddAnimation()` para registrar as animações do Mario Grande (ex: `"idle"`, `"run"`, `"jump"`).
    * Chama `mAnimator->SetAnimation()` para definir a animação correta com base no estado atual do Mario (correndo, pulando, parado).
    * Ajusta a escala visual do `Actor` (`SetScale()`) para dobrar a altura.
    * Ajusta a posição do `Actor` (`SetPosition()`) ligeiramente para cima, para que o crescimento pareça vir da base.
    * **Ajusta o Colisor:** Chama `mCollider->SetDimensions()` e `mCollider->SetOffset()` para aumentar a altura do `AABBColliderComponent` e reposicioná-lo verticalmente, alinhado com o novo tamanho visual.
    * Destrói o objeto `PowerUp` coletado (`powerUp->SetState(Actor::State::Destroy)`).

### 2. Encolhimento (`Mario::Shrink()`)

* **Gatilho:** Chamado em `Mario::OnHorizontalCollision` ou `Mario::OnVerticalCollision` quando Mario colide com um `Actor` na camada `ColliderLayer::Enemy` **e** `mIsBig` é `true` **e** `mIsInvulnerable` é `false`.
* **Ação:**
    * Verifica se Mario já está pequeno (`!mIsBig`) ou invulnerável (`mIsInvulnerable`). Se sim, a função retorna.
    * Define o estado interno `mIsBig = false`.
    * **Ativa a Invulnerabilidade:** Define `mIsInvulnerable = true` e inicializa `mInvulnerabilityTimer = INVULNERABILITY_DURATION`.
    * Chama `mAnimator->LoadSheet()` para recarregar a folha de sprites e JSON do Mario Pequeno.
    * Chama `mAnimator->AddAnimation()` para registrar novamente as animações do Mario Pequeno (ex: `"idle"`, `"run"`, `"jump"`, `"dead"`).
    * Chama `mAnimator->SetAnimation()` para a animação correta do estado atual.
    * Ajusta a escala visual (`SetScale()`) de volta ao tamanho original.
    * Ajusta a posição (`SetPosition()`) ligeiramente para baixo.
    * **Ajusta o Colisor:** Chama `mCollider->SetSize()` e para retornar o colisor ao tamanho e posição originais do Mario Pequeno.

### 3. Invulnerabilidade

* **Ativação:** Ocorre automaticamente ao chamar `Mario::Shrink()`.
* **Duração:** Controlada pela constante `INVULNERABILITY_DURATION` (em segundos).
* **Efeito:**
    * Dentro de `Mario::OnHorizontalCollision` e `Mario::OnVerticalCollision`, a verificação `if (!mIsInvulnerable)` impede que `Shrink()` ou `Kill()` sejam chamados novamente se Mario colidir com um inimigo.
    * Dentro de `Mario::OnUpdate()`:
        * O `mInvulnerabilityTimer` é decrementado por `deltaTime`.
        * Um efeito visual de **piscar** é aplicado alterando a visibilidade do `AnimatorComponent` (`mAnimator->SetVisible()`) com base no tempo restante.
        * Quando `mInvulnerabilityTimer` chega a zero ou menos, `mIsInvulnerable` é definido como `false`, e a visibilidade do Animator é garantida como `true`.

## Bloco Padrão (`Block`) e Comportamento de Quique (Bloco de Tijolo "Tipo B")

* **Herança:** `Block : public Actor`
* **Propósito:** Representa os blocos estáticos do cenário, incluindo os blocos de tijolo que podem quicar quando atingidos por baixo.
* **Identificação (Tipo B):** A própria classe `Block` contém a lógica para identificar se é um bloco de tijolo. No construtor (`Block::Block()`), ela recebe o `nome` vindo do `Game::BuildLevel`. Uma verificação interna (`if (nome == BLOCO B)`) define a flag `mCanBounce = true` se o ID corresponder ao Bloco de Tijolo. Blocos que não são do Tipo B terão `mCanBounce = false`.
* **Comportamento de Quique:**
    * **Gatilho:** Quando o Mario colide verticalmente *de baixo para cima* (`overlap > 0.0f`) com um `Actor` na camada `ColliderLayer::Blocks`, `Mario::OnVerticalCollision` faz um `dynamic_cast<Block*>` para obter um ponteiro para o bloco atingido. Se for bem-sucedido, ele chama `block->StartBounce()`.
    * **`Block::StartBounce()`:**
        * Este método existe na classe `Block`.
        * Ele primeiro verifica se `mCanBounce` é `true` e se o bloco já não está quicando (`!mIsBouncing`). Se uma dessas condições falhar, ele retorna sem fazer nada (garantindo que blocos normais não quiquem).
        * Se puder quicar, define `mIsBouncing = true` e reseta `mBounceTimer = 0.0f`.
        * Armazena a `mOriginalPosition` (se ainda não foi definida).
    * **`Block::OnUpdate()`:**
        * Este método também existe na classe `Block`.
        * Contém uma seção que só executa se `mIsBouncing` for `true`:
            * Incrementa `mBounceTimer` com `deltaTime`.
            * Usa `Math::Lerp` (interpolação linear) para calcular um deslocamento vertical (`offsetY`) baseado no `mBounceTimer` e na `BOUNCE_DURATION`. O deslocamento simula o movimento para cima e depois para baixo.
            * Define a posição atual do bloco como `mOriginalPosition + Vector2(0.0f, offsetY)`.
            * Quando `mBounceTimer` ultrapassa `BOUNCE_DURATION`, define `mIsBouncing = false` e reposiciona o bloco exatamente em `mOriginalPosition`.


## Novas Classes de Atores

### 1. Bloco de Interrogação (`QuestionBlock`)

* **Herança:** `QuestionBlock : public Block`
* **Propósito:** Representa os blocos "?" que podem ser atingidos por baixo para liberar um item.
* **Comportamento:**
    * Identificado em `Game::BuildLevel` pelo seu `tileID` específico.
    * Possui um estado interno `mIsEmpty` (inicialmente `false`).
    * Sobrescreve `Block::StartBounce()`: Só permite o início do quique se `mIsEmpty` for `false`.
    * Sobrescreve `Block::OnUpdate()`:
        * Chama `Block::OnUpdate()` para executar a animação de quique.
        * Detecta o **final** da animação de quique (usando a flag `mBounceTriggered` e `!mIsBouncing`).
        * Se o quique acabou e `mIsEmpty` ainda é `false`:
            * Define `mIsEmpty = true`.
            * Pega o `AnimatorComponent` e chama `SetTexture()` para mudar para a textura do bloco vazio (`mEmptyBlockTexture`, carregada no construtor).
            * **Em blocos específicos com o o nome "Block Mushroom" instancia um novo `Mushroom`** (`new Mushroom(...)`) na posição acima do bloco.

### 2. Cogumelo (`Mushroom`)

* **Herança:** `Mushroom : public Actor`
* **Propósito:** Representa o power-up que faz o Mario crescer.
* **Criação:** Instanciado por `QuestionBlock::OnUpdate()`.
* **Comportamento:**
    * **Animação de Saída:** Possui um estado `mIsPoppingUp` (inicialmente `true`). No `OnUpdate()`, enquanto `mIsPoppingUp` for `true`, o cogumelo se move manualmente para cima (`pos.y -= moveAmount`) a uma velocidade `POP_UP_SPEED` até atingir a `POP_UP_TARGET`. A gravidade no `RigidBodyComponent` está desligada durante esta fase.
    * **Transição para Física:** No quadro exato em que atinge `POP_UP_TARGET`, `mIsPoppingUp` vira `false`, a gravidade (`mRigidBody->SetApplyGravity(true)`) é ativada, e uma velocidade horizontal inicial (`mRigidBody->SetVelocity(Vector2(mWalkSpeed, 0.0f))`) é definida.
    * **Movimento:** Após sair, move-se horizontalmente com `mWalkSpeed`. A gravidade o puxa para baixo.
    * **Colisão:**
        * `OnHorizontalCollision()`: Inverte a `mWalkSpeed` ao colidir com `ColliderLayer::Blocks`.
        * É ignorado por `ColliderLayer::Enemy` (verificação em `AABBColliderComponent::Detect...Collision`).
        * Ao colidir com `ColliderLayer::Player` (Mario), chama `SetState(ActorState::Destroy)` em si mesmo (é coletado).
    * **Destruição:** Chama `SetState(ActorState::Destroy)` se cair para fora dos limites inferiores da tela (`OnUpdate()`).