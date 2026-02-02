🧠 Projeto: Jogo WAR em C com Sistema de Missões

Este projeto consiste no desenvolvimento de um jogo inspirado no WAR, implementado em linguagem C, com foco em lógica de programação, uso de ponteiros, estruturas de dados, alocação dinâmica de memória e modularização de código.

O sistema simula batalhas entre territórios, utilizando rolagem de dados aleatórios, e introduz um sistema de missões dinâmicas, tornando o jogo estratégico e progressivo.

🎯 Principais Funcionalidades

Cadastro inicial de territórios com:

Nome

Cor do exército

Quantidade de tropas

Sistema de batalha com dados (1 a 6), simulando confrontos entre territórios

Conquista de territórios com:

Troca de cor do território conquistado

Retorno ao jogo com tropas iniciais

Exibição contínua do mapa do mundo atualizado

Menus interativos para ações do jogador

🧩 Sistema de Missões

O jogo conta com um sistema de missões aleatórias, atribuídas dinamicamente ao jogador:

Eliminar todas as tropas de uma cor sorteada a partir do cadastro

Vencer três batalhas seguidas (com controle de empates)

Reduzir um território inimigo a zero tropas

Comportamento das missões:

Missões são exibidas apenas uma vez, no início

A verificação ocorre silenciosamente ao longo do jogo

Algumas missões permitem:

Continuação do jogo com nova missão

Reset completo do jogo

Encerramento da partida

Empates em batalhas não contam como vitória para fins de missão

🧪 Regras de Jogo Implementadas

Empate nos dados não altera tropas nem contadores

Contador de vitórias consecutivas controlado corretamente

Missão “Reduzir território a zero” gera automaticamente uma nova missão

Menu especial após concluir a missão “Vencer três batalhas seguidas”

Pausas estratégicas para melhor visualização dos resultados

🛠️ Conceitos Técnicos Utilizados

struct para modelagem de territórios

Ponteiros e passagem por referência

malloc e calloc para alocação dinâmica

Modularização com funções específicas

Manipulação de strings (strcpy, strcmp, strstr)

Geração de números aleatórios (rand, srand)

Controle de fluxo com menus e estruturas condicionais

Boas práticas de liberação de memória (free)

📚 Objetivo Educacional

Este projeto foi desenvolvido com fins didáticos, como parte do aprendizado em programação em C, visando consolidar conceitos fundamentais de programação estruturada, gestão de memória, lógica de jogos e organização de código.

🚀 Considerações Finais

O projeto demonstra a evolução da aplicação desde um jogo simples até um sistema mais robusto, com regras claras, missões dinâmicas e interação com o usuário, sendo um excelente exemplo prático para estudos e portfólio em Desenvolvimento de Software.
