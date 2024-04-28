#include <stdio.h>
#include <stdlib.h>

// Structura pentru nodul grafului
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structura pentru graful în sine
typedef struct Graph {
    int vertices;
    int* visited;
    struct Node** adjacency_lists;
} Graph;

// Funcție pentru crearea unui nod
Node* create_node(int v) {
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Eroare la alocarea memoriei pentru nod.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

// Funcție pentru crearea grafului
Graph* create_graph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Eroare la alocarea memoriei pentru graf.\n");
        exit(EXIT_FAILURE);
    }
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(Node*));
    graph->visited = malloc(vertices * sizeof(int));
    if (graph->adjacency_lists == NULL || graph->visited == NULL) {
        printf("Eroare la alocarea memoriei pentru lista de adiacență sau vectorul de vizitare.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Funcție pentru adăugarea unei muchii între două noduri
void add_edge(Graph* graph, int src, int dest) {
    Node* new_node = create_node(dest);

    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);

    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

// Funcție pentru introducerea muchiilor în graf
void insert_edges(int nr_of_vertices, int nr_of_edges, Graph* graph) {
    int src, dest;
    printf("Adauga %d muchii (de la 1 la %d)\n", nr_of_edges, nr_of_vertices);
    for (int i = 0; i < nr_of_edges; i++) {
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

// Funcție pentru verificarea dacă o coadă este goală
int is_empty(Node* queue) {
    return queue == NULL;
}

// Funcție pentru adăugarea unui element în coadă
void enqueue(Node** queue, int data) {
    Node* new_node = create_node(data);

    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        Node* temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Funcție pentru eliminarea unui element din coadă și returnarea sa
int dequeue(Node** queue) {
    int data = (*queue)->data;
    Node* temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

// Funcție pentru parcurgerea grafului folosind DFS
void DFS(Graph* graph, int vertex) {
    Node* adj_list = graph->adjacency_lists[vertex];
    Node* temp = adj_list;

    graph->visited[vertex] = 1;
    printf("%d->", vertex);

    while (temp != NULL) {
        int connected_vertex = temp->data;

        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

// Funcție pentru parcurgerea grafului folosind BFS
void BFS(Graph* graph, int start) {
    Node* queue = NULL;

    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        Node* temp = graph->adjacency_lists[current];
        while (temp) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

// Funcție pentru inițializarea vectorului de vizitare cu 0
void wipe_visited_list(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

int main() {
    int nr_of_vertices, nr_of_edges, starting_vertex;
    printf("Cate noduri are graful? ");
    scanf("%d", &nr_of_vertices);
    printf("Cate muchii are graful? ");
    scanf("%d", &nr_of_edges);

    Graph* graph = create_graph(nr_of_vertices);
    insert_edges(nr_of_vertices, nr_of_edges, graph);

    printf("De unde plecam in DFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex);
    wipe_visited_list(graph);

    printf("\nDe unde plecam in BFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex);

    // Eliberarea memoriei
    for (int i = 0; i < graph->vertices; i++) {
        Node* temp = graph->adjacency_lists[i];
        while (temp != NULL) {
            Node* del = temp;
            temp = temp->next;
            free(del);
        }
    }
    free(graph->adjacency_lists);
    free(graph->visited);
    free(graph);

    return 0;
}
