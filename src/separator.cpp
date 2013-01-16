#include "separator.h"

namespace chordalg {


Separator::Separator(Graph* const G) : G_(G), component_of_vertex_(), size(0) {
    component_of_vertex_.resize(G->order());
    return;
}

Separator::~Separator() {}

// Calculate connected components of G - S
// ToDo: Write BFS algorithm as a function?
void Separator::Calculate(VertexContainer S) {
    return;
    std::queue< Vertex > search_queue;
    ConnectedComponentOf current_component = first_component_ - 1;

    // initialize vertex labels
    for(Vertex &v : component_of_vertex_)
        v = unsearched_;
    for(Vertex const &v : S)
        component_of_vertex_[v] = in_separator_;

    // BFS to find connected components
    for(Vertex const& v : *G_){ // in order for this to work, begin() needs to return an iterator to a vertex set
    //for(Vertex &v = G_->begin(); v != G_->end(); ++v){
        if(component_of_vertex_[v] == unsearched_)
        {
            search_queue.push(v);
            ++current_component;
        } // if new connected component found

        while(!search_queue.empty()){
            // pop vertex and mark found
            Vertex u = search_queue.front();
            search_queue.pop();
            component_of_vertex_[u] = current_component;

            // BFS on u's neighborhood
            for(Vertex w : G_->N(u)){
            //for(VertexIterator vtr = G_->N(u).begin(); vtr != G_->N(u).end(); ++vtr){
                if(component_of_vertex_[w] == unsearched_)
                    search_queue.push(w);
            }
        }
    }

    size = current_component;
    return;
}

} // namespace chordalg
