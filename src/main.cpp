#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <limits.h>

#include "../include/node.h"

// constants for fragments (KB)
#define full_mem 2000
#define fr_mem 4
#define fr_num full_mem/fr_mem

using namespace std;

void* mem[fr_num];
int node_num = 0;
int max_nodes_in_fragment;
node* root;
map<string, int> mm;

void initialize_mem()
{
    for (int i = 0; i < fr_num; i++)
    {
        mem[i] = malloc(fr_mem * 1024);

        // free fragment
        for(int j = 0; j < fr_mem * 1024; j++)
            *((char*)(mem[i] + j)) = 0;
    }

//    for (int i = 0; i < fr_mem * 1024; i++)
//    {
//        printf("%d\n", *((char*)(mem[8] + i)));
//    }
}

/*
    Find First Free Field
*/
node* fff_field()
{
    // finding fragment with some nodes already, but is not filled
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) < max_nodes_in_fragment && *((short*)(mem[i])) > 0 )
        {
            *((short*)(mem[i])) = *((short*)(mem[i])) + 1;
            for (int j = 0; j < max_nodes_in_fragment; j++)
            {
                if ( *((char*)(mem[i] + 2 + j * (sizeof(node) + 1))) == 0 )
                {
                    *((char*)(mem[i] + 2 + j * (sizeof(node) + 1))) = 1; // filled field with some node
                    return (node*)(mem[i] + 3 + j * (sizeof(node) + 1));
                }
            }
        }
    }

    // finding free fragment if necessary
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) == 0 )
        {
            *((short*)(mem[i])) = *((short*)(mem[i])) + 1;
            *((char*)(mem[i] + 2)) = 1; // filled field with some node
            return (node*)(mem[i] + 3);
        }
    }
}

/*
    Find First Free Fragment
*/
int fff_fragment()
{
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) == 0 )
        {
            *((short*)(mem[i])) = SHRT_MAX;
            *((void**)(mem[i] + fr_mem * 1024 - 4)) = NULL;
            return i;
        }
    }

    return -1;
}

node* create_node(char* name, bool folder, node* parent, node* older_from, node* older_to)
{
    // finding first free field of free memory
    node* beg;
    beg = fff_field();

    // given parameters
    strcpy(beg->name, name);
    beg->folder = folder;
    beg->parent = parent;
    beg->older_from = older_from;
    beg->older_to = older_to;

    beg->indx = node_num;
    node_num++;

    // when created
    beg->younger = NULL;
    beg->start = NULL;

    return beg;
}

void delete_node(node* del)
{
    *((char*)(del - 1)) = 0; // is not filled with node anymore
}

node* make_root()
{
    node* root = create_node("R", true, NULL, NULL, NULL);
    root->parent = root;
    root->older_from = root;
    return root;
}

/*
    List of commands with codes:
        exit - 1
        ls - 2
        cd - 3
        mkdir - 4
        touch - 5
        rm - 6
*/
void map_commands()
{
    mm["exit"] = 1;
    mm["ls"] = 2;
    mm["cd"] = 3;
    mm["mkdir"] = 4;
    mm["touch"] = 5;
    mm["rm"] = 6;
}

/*
    List of commands:
        exit
            code: 1
            ex: root exit
        ls (read-only) - List Directory - list everything that is inside selected directory
            code: 2
            ex: root ls
                list everything in folder root
        cd (read-only) - Change Directory - go in selected directory
            code: 3
            ex: root cd my-folder
                go to my-folder if it exists in folder root
        mkdir - Make Directory - make new directory
            code: 4
            ex: root mkdir my-folder
                make folder my-folder in folder root
        touch - making new file
            code: 5
            ex: root touch my-file.txt
                create new file whose name is 'my-file.txt'
        rm - Remove - delete selected file or folder
            code: 6
            ex: root rm my-folder
                delete my-folder and recursive everything inside
*/
void read_commands()
{
    map_commands();

    char command[10];
    bool running = true;

    do
    {
        scanf("%s", command);

        switch ( mm[command] )
        {
        case 1:
            {
                // exit
                running = false;
            } break;
        case 2:
            {
                // ls
            } break;
        case 3:
            {
                // cd
            } break;
        case 4:
            {
                // mkdir
            } break;
        case 5:
            {
                // touch
            } break;
        case 6:
            {
                // rm
            } break;
        default:
            {
                // wrong command
                printf("Wrong command!\n");
                // recommend help
            }
        }
    }
    while(running);
}

void test_fff_fragment()
{
    printf("%d\n", fff_fragment()); // 1
}

int main()
{
    initialize_mem();

    max_nodes_in_fragment = (fr_mem - 2) / (sizeof(node) + 1);
    root = make_root();

    // testing fff_fragment
    test_fff_fragment();

    read_commands();

    return 0;
}