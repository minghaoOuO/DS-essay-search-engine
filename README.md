# DS-essay-search-engine
2021 aultumn

## this is a essay search engine!!! 
using trie and ternary search tree to build dictionary trees.Give it essays and some search words, it will return those essay with the given words

### bellow are types of search can be choosed!!
1. exact search :"search-word"
2. prefix search :seaarch-word
3. suffix search \*search-word\*

### And we can concate the result of multiple search
1. and operator +
2. or operator /


### the example query
1. “graph” / *composition*

Essay set A: Find essays that have exactly the word [graph]

Essay set B: Find essays that have words with suffix [composition]

A, B set with OR operator -> answer = union of sets A and B

2. graph + decomposition / reflection

Essay set A: Find essays that have words with prefix [graph]

Essay set B: Find essays that have words with prefix [decomposition]

Essay set C: Find essays that have words with prefix [reflection]

We know that A + B / C = (A + B) / C
Essay set D = intersection of sets A and B
Answer = union of sets D and C
