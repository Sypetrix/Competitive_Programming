# Complete Search (Força-Bruta)

A técnica de Busca Completa (Complete Search), também conhecida como força bruta ou backtracking recursivo, é um método que resolve problemas percorrendo todo (ou parte) do espaço de busca para encontrar a solução. 
*Durante esse processo, é possível "podar" (prune) caminhos que claramente não contêm a resposta.*

**Se a solução existir, a Busca Completa sempre a encontrará.**

### Na programação competitiva, você deve usar a Busca Completa quando:

- **Não há outro algoritmo disponível (ex: listar todas as permutações).**
- **Existem algoritmos melhores, mas a entrada é muito pequena, tornando uma abordagem simples suficiente.**

A Busca Completa deve ser a primeira ideia a ser considerada, pois é fácil de criar, programar e debugar. Lembre-se do princípio `KISS` mencionado anteriormente (`Keep It Short and Simple - Mantenha Curto e Simples`).
Um código de Busca Completa sem erros nunca receberá Wrong Answer (`WA`), pois ele testa todas as possibilidades. No entanto, por ser lento, pode receber Time Limit Exceeded (`TLE`).
Antes de programar, analise o tamanho da entrada para ver se a solução passará no tempo limite. Se sim, implemente-a e ganhe tempo para os problemas mais difíceis.

### Outros usos:

- **Encontrar Padrões:** rodar a Busca Completa em exemplos pequenos pode revelar padrões na saída, ajudando a criar um algoritmo mais rápido.
- **Validação:** ela serve como um verificador seguro para testar se um algoritmo mais rápido e complexo está funcionando corretamente em casos menores.

*Embora pareça uma técnica apenas para problemas "fáceis", existem problemas muito difíceis (como os NP-completos) que só podem ser resolvidos com algoritmos criativos de Busca Completa.*