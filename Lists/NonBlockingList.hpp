#include <Node.h>

class NonBlockingList {
   public:
    NonBlockingList() {
        head = new Node();
        tail = new Node();
        head->next = tail;
    }

    bool insert(int value);
    bool remove(int value);
    bool find(int value);

   private:
    // sentinel
    Node* head;
    Node* tail;

    Node* search(int value, Node*& leftNode);

    /*
        A node is marked if and only if its next field is marked.
        Marked references are distinct from normal references but still allow
        the referred-to node to be determined for example, they may be indicated
        by an otherwise-unused low-order bit in each reference.
    */
    // is_marked_reference(r) returns true if and only if r is a marked
    // reference.
    bool is_marked_reference(Node* node) { return ((long)node & 0x1L); }

    Node* get_marked_reference(Node* node) { return (Node*)((long)node & ~0x1L); }

    Node* get_unmarked_reference(Node* node) { return (Node*)((long)node | 0x1L); }

};


bool NonBlockingList::insert(int value) {
    Node* new_node = new Node(value);
    Node*right_node, *left_node;
    do {
        // locate the pair of nodes between which the new node is to be
        // inserted.
        right_node = search(value, left_node);
        if ((right_node != tail) && (right_node->value == value)) /*T1*/
            return false;
        new_node->next = right_node;
        if (__sync_bool_compare_and_swap(&(left_node->next), right_node, new_node)) /*C2*/
            return true;
    } while (true); /*B3*/
}


bool NonBlockingList::remove(int search_value) {
    Node *right_node, *right_node_next, *left_node;
    do {
        right_node =
            search(search_value, left_node);  // locate the node to delete
        if ((right_node == tail) || (right_node->value != search_value)) /*T1*/
            return false;
        right_node_next = right_node->next;
        if (!is_marked_reference(right_node_next))
            if (__sync_bool_compare_and_swap(&(right_node->next), right_node_next,
                    get_marked_reference(
                        right_node_next))) /*C3*/  // the node is logically
                                                   // deleted
                break;
    } while (true); /*B4*/

    if (!__sync_bool_compare_and_swap(&(left_node->next), right_node,
             right_node_next)) /*C4*/  // the node is physically deleted
        right_node = search(right_node->value, left_node);
    return true;
}


bool NonBlockingList::find(int search_value) {
    Node*right_node, *left_node;
    right_node = search(search_value, left_node);
    if ((right_node == tail) || (right_node->value != search_value))
        return false;
    else
        return true;
}


Node* NonBlockingList::search(int search_value, Node*& left_node) {
    /*
        It takes a search value and returns references to two nodes called the
       left node and right node for that value. Conditions need to be satisfied:
        1. the value of the left node must be less than the search value
            and the value of the right node must be greater than or equal to the
       search value.
        2. both nodes must be unmarked.
        3. the right node must be the immediate successor of the left node.

    */
    Node*left_node_next, *right_node;
search_again:
    do {
        Node* t = head;
        Node* t_next = head->next;

        /* Step 1: Find left_node and right_node */
        do {
            if (!is_marked_reference(t_next)) {  // t_next is not the node that
                                                 // needs to be deleted
                left_node = t;
                left_node_next = t_next;
            }
            t = get_unmarked_reference(t_next);
            if (t == tail)
                break;
            t_next = t->next;
        } while (is_marked_reference(t_next) || (t->value < search_value)); /*B1*/
        right_node = t;

        /* Step 2: Check nodes are adjacent */
        if (left_node_next == right_node)
            if ((right_node != tail) &&
                is_marked_reference(right_node->next))  // right_node.next is
                                                       // marked to be deleted
                goto search_again;                     /*G1*/
            else
                return right_node; /*R1*/

        /* Step 3: Remove one or more marked nodes */
        if (__sync_bool_compare_and_swap(&(left_node->next), left_node_next, right_node)) /*C1*/
            if ((right_node != tail) && is_marked_reference(right_node->next))
                goto search_again; /*G2*/
            else
                return right_node; /*R2*/
    } while (true);                /*B2*/
}
