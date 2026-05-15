#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define VERSION "1.0.0"

typedef struct
{
    const char *id;
    const char *description;
    const char *filename;
    const char *content;
}Template;

Template templates[] = 
{
    {
        "c",
        "C source code",
        "main.c",
        "#include <stdio.h>\n\n"
        "int main()\n"
        "{\n"
        "    printf(\"hello\\n\");\n\n"
        "    return 0;\n"
        "}\n"
    },
    {
        "py",
        "Python 3 source code",
        "main.py",
        "#!/usr/bin/env python3\n\n"
        "def main():\n"
        "    print(\"Py3\")\n\n"
        "if __name__ == \"__main__\":\n"
        "    main()\n"
    },
    {
        "sh",
        "Bash source code",
        "main.sh",
        "#!/usr/bin/env bash\n\n"
        "echo \"hello\"\n"
    },
    {
        "java",
        "Java source code",
        "Main.java",
        "public class Main {\n"
        "    public static void main(String[] args) {\n"
        "        System.out.println(\"hello\");\n"
        "    }\n"
        "}\n"
    }
};

int template_count = sizeof(templates) / sizeof(templates[0]);

void print_help(void)
{
    printf("alap v%s\n\n", VERSION);
    printf("Usage: alap <template_id> [option]\n\n");

    printf("Available options:\n\n");
    printf("-h, --help       show this help\n");
    printf("-v, --version    version info\n");
    printf("--stdout         don't create source file, print result to stdout\n\n");

    printf("Available templates:\n\n");

    for(int i = 0; i < template_count; i++) 
    {
        printf("* %-8s - %s [%s]\n",
               templates[i].id,
               templates[i].description,
               templates[i].filename);
    }
}

Template *find_template(const char *id)
{
    for(int i = 0; i < template_count; i++) 
    {
        if(strcmp(templates[i].id, id) == 0) 
        {
            return &templates[i];
        }
    }

    return NULL;
}

bool file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if(file != NULL) 
    {
        fclose(file);
        return true;
    }

    return false;
}

int create_file(const char *filename, const char *content)
{
    if(file_exists(filename)) 
    {
        fprintf(stderr, "Error: the file '%s' already exists\n", filename);
        return 1;
    }

    FILE *file = fopen(filename, "w");

    if(file == NULL) 
    {
        fprintf(stderr, "Error: could not create file '%s'\n", filename);
        return 1;
    }

    fprintf(file, "%s", content);
    fclose(file);

    printf("# `%s` was created\n", filename);

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 1) 
    {
        print_help();
        return 0;
    }

    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) 
    {
        print_help();
        return 0;
    }

    if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) 
    {
        printf("alap v%s\n", VERSION);
        return 0;
    }

    Template *selected = find_template(argv[1]);

    if(selected == NULL) 
    {
        fprintf(stderr, "Error: unknown template '%s'\n\n", argv[1]);
        print_help();
        return 1;
    }

    bool stdout_mode = false;

    for(int i = 2; i < argc; i++) 
    {
        if(strcmp(argv[i], "--stdout") == 0) 
        {
            stdout_mode = true;
        }
        else 
        {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            return 1;
        }
    }
    Template sablon = *selected;

    if(stdout_mode)
    {
        printf("%s", sablon.content);
        return 0;
    }

    return create_file(sablon.filename,sablon.content);
}