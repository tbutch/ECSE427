
void parseInput(char * wordArray[]){
    int i = 0;
    // While the input words are not null
    while(strcmp(wordArray[i], "\0")){
        printf("%d. %s\n", i+1, wordArray[i]);
        i++;

    }
}