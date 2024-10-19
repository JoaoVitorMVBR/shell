#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <sys/wait.h>

#define SH_RL_BUFSIZE 1024
#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a"

void sh_loop(void);
char *sh_read_line(void);
char **sh_split_line(char *line);
int sh_launch(char **args);
int sh_execute(char **args);

//shell comands
int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &sh_cd,
  &sh_help,
  &sh_exit
};

int sh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}
//builtin functions implementation
int sh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < sh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return sh_launch(args);
}

int sh_cd(char **args){
    
    char prev_dir[SH_RL_BUFSIZE];

    if (args[1] == NULL) {
        char *home = getenv("HOME");
        if (home == NULL) {
            fprintf(stderr, "sh: HOME not set\n");
        } else {
            if (chdir(home) != 0) {
                perror("sh");
            }
        }
    }

    else if (strcmp(args[1], "~") == 0){
        char *home = getenv("HOME");
        if (home == NULL) {
            fprintf(stderr, "sh: HOME environment variable not set\n");
        } else {
            if (chdir(home) != 0) {
                perror("sh");
            }
        }
    }

    else if(strcmp(args[1], "-") == 0){
        if (prev_dir[0] == '\0'){
            fprintf(stderr, "sh: DIR not set");
        }
        else{
            if (chdir(prev_dir) != 0) {
                perror("sh");
            } 
            else {
                printf("%s\n", prev_dir);  // Exibe o diretório para onde mudou
            }
        }
    }

    else{
        char current_dir[SH_RL_BUFSIZE];
        if(getcwd(current_dir, sizeof(current_dir)) == NULL){
            perror("getcwd error");
        }
        if(chdir(args[1]) != 0){
            perror("sh aqui");
        }
        else{
            strcpy(prev_dir, current_dir);
        }
    }
    return 1;
}

int sh_help(char **args){
  int i;
  printf("João Vilas Boas\n");

  for (i = 0; i < sh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("above the list of programs.\n");
  return 1;
}

int sh_exit(char **args){
  return 0;
}

// lifecicle function and parse strings
int sh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid==0){
        if (execvp(args[0], args) == -1){
            perror("sh");
        }
        exit(EXIT_FAILURE);
    }else if(pid < 0){
        perror("sh");
    }else{
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

char **sh_split_line(char *line){
    int bufsize = SH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    char *token;

    if(!tokens){
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position ++;

        //if we exceed the array of tokens
        if(position >= bufsize){ 
            bufsize += SH_TOK_BUFSIZE;
            char **temp = realloc(tokens, bufsize * sizeof(char*));
            if(!temp){
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
            tokens = temp;
        }
        token = strtok(NULL, SH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

char *sh_read_line(void){
    int bufsize = SH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if(!buffer){
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();
        //if the command end
        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        //if the command continue
        }else{
            buffer[position] = c;
        }
        position++;

        //if we have exceed the buffer
        if(position >= bufsize){
            bufsize += SH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if(!buffer){
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void sh_loop(void){
    char *line;
    char **args;
    int status;

    do{
        printf("> ");
        line = sh_read_line();
        args = sh_split_line(line);
        status = sh_execute(args);
        free(line);
        free(args);
    }while(status);
}

int main(int argc, char **argv){

    sh_loop();

    return 0;
}