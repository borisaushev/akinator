#include "treeDump.h"

const char* colors[] = {
    "red", "blue", "purple", "brown", "darkcyan"
};

const char* getPointerColor(void* ptr) {
    size_t color_index = (size_t)ptr % (sizeof(colors) / sizeof(colors[0]));
    return colors[color_index];
}

static void addNodeInfo(FILE* file, int index, treeNode_t* node) {
    fprintf(file, "Node_%d [shape=Mrecord; style=filled; fillcolor = \"#56db70\"; "
                  "color = \"black\"; "
                  "label = <{ptr: <font color=\"%s\">%p</font> | %s | ",
            index, getPointerColor(node), node, getData(node));

    if (getLeft(node) != NULL) {
        fprintf(file, "{L: <font color=\"%s\">%p</font>",
                getPointerColor(getLeft(node)), getLeft(node));
    }
    else {
        fprintf(file, "{L: <font color=\"blue\">NULL</font>");
    }

    if (getRight(node) != NULL) {
        fprintf(file, " | R: <font color=\"%s\">%p</font>}}>; ]\n",
                getPointerColor(getRight(node)), getRight(node));
    }
    else {
        fprintf(file, " | R: <font color=\"blue\"> NULL</font>}}>; ]\n");
    }
}

static void addNodesRec(FILE* file, treeNode_t* curNode, int* counter) {
    int curIndex = *counter;

    treeNode_t* children[] = {getLeft(curNode), getRight(curNode)};
    for (int i = 0; i < 2; i++) {
        treeNode_t* child = children[i];
        if (child != NULL) {
            *counter = *counter + 1;
            addNodeInfo(file, *counter, child);
            fprintf(file, "Node_%d->Node_%d\n", curIndex, *counter);
            addNodesRec(file, child, counter);
        }
    }
}

static void generateDotFile(treeNode_t* node, FILE *dotFile) {
    assert(dotFile);
    fprintf(dotFile, "digraph G {\n");

    int counter = 0;
    addNodeInfo(dotFile, 0, node);
    addNodesRec(dotFile, node, &counter);

    fprintf(dotFile, "}\n");
}

static void fillHtmlHeader(const char *desc, const char *fileName, const int line,
                           const char *func, error_t code, FILE* htmlFile, size_t counter) {
    assert(htmlFile);
    // заголовок
    fprintf(htmlFile, "</p><h3>DUMP#%llu <font color=red>%s</font> %s "
                      "at (%s:%d) - STATUS: %d</h3></p>\n",
            counter, desc, func, fileName, line, code);
}

error_t treeDump(treeNode_t* node, const char* desc, const char* file,
                 const int line, const char* func, error_t code) {
    static size_t counter = 0;

    static FILE* html_file = fopen(HTML_FILE_PATH, "w");

    if (!html_file) {
        RETURN_ERR(CANT_OPEN_FILE, "Cannot open HTML log file");
    }

    char svg_filename[256] = {};
    snprintf(svg_filename, sizeof(svg_filename), SVG_FORMAT, counter);
    fillHtmlHeader(desc, file, line, func, code, html_file, counter);

    // DOT файл
    FILE* dot_file = fopen(DOT_FILE_PATH, "w");
    if (!dot_file) {
        fclose(html_file);
        RETURN_ERR(CANT_OPEN_FILE, "cannot create DOT file");
    }
    generateDotFile(node, dot_file);
    fclose(dot_file);

    // Генерируем PNG
    char command[512];
    snprintf(command, sizeof(command), "dot -T svg %s -o %s",
             DOT_FILE_PATH, svg_filename);
    int result = system(command);

    if (result != 0) {
        fprintf(html_file, "<p style=\"color: red;\">Graphviz generation failed!</p>\n");
    } else {
        fprintf(html_file, "<img src=\"images/tree_%zu.svg\" "
                           "alt=\"List structure visualization\">\n",
                counter);
    }

    fprintf(html_file, "<hr>\n");

    counter++;
    fflush(html_file);

    return SUCCESS;
}
