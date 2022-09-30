/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#pragma once

#include <ManifoldTopologies/config.h>
#include <sofa/component/topology/container/dynamic/EdgeSetTopologyContainer.h>

namespace sofa::component::topology::container::dynamic
{
class ManifoldEdgeSetTopologyModifier;

using core::topology::BaseMeshTopology;

typedef BaseMeshTopology::PointID			PointID;
typedef BaseMeshTopology::EdgeID			EdgeID;
typedef BaseMeshTopology::Edge				Edge;
typedef BaseMeshTopology::SeqEdges			SeqEdges;
typedef BaseMeshTopology::EdgesAroundVertex		EdgesAroundVertex;

/** a class that stores a set of edges and provides access to the adjacency between points and edges.
  this topology is constraint by the manifold property : each vertex is adjacent either to one vertex or to two vertices. */
class SOFA_MANIFOLD_TOPOLOGIES_API ManifoldEdgeSetTopologyContainer : public EdgeSetTopologyContainer
{
    friend class ManifoldEdgeSetTopologyModifier;

public:
    SOFA_CLASS(ManifoldEdgeSetTopologyContainer,EdgeSetTopologyContainer);

    using Index = sofa::Index;

    ManifoldEdgeSetTopologyContainer();

    ~ManifoldEdgeSetTopologyContainer() override {}

    void init() override;

    void clear() override;

    /** \brief Checks if the topology is coherent
    *
    * Check if the shell arrays are coherent
    */
    bool checkTopology() const override;

    /** \brief Returns the number of connected components from the graph containing all edges and give, for each vertex, which component it belongs to  (use BOOST GRAPH LIBRAIRY)
    @param components the array containing the optimal vertex permutation according to the Reverse CuthillMckee algorithm
    */
    virtual int getNumberConnectedComponents(sofa::type::vector<Index>& components) override;

protected:
    /** \brief Creates the EdgeSetIndex.
    *
    * This function is only called if the EdgeShell member is required.
    * EdgeShell[i] contains the indices of all edges having the ith DOF as
    * one of their ends.
    */
    void createEdgesAroundVertexArray() override;

private:
    // Describe each connected component, which can be seen as an oriented line
    class ConnectedComponent
    {
    public:

        ConnectedComponent(Index FirstVertexIndex=-1, Index LastVertexIndex=-1, Index size=0,Index ccIndex=0)
            : FirstVertexIndex(FirstVertexIndex), LastVertexIndex(LastVertexIndex), size(size), ccIndex(ccIndex)
        {}

        virtual ~ConnectedComponent() {}

    public:
        Index FirstVertexIndex; // index of the first vertex on the line
        Index LastVertexIndex; // index of the last vertex on the line

        Index size; // number of the vertices on the line

        Index ccIndex; // index of the connected component stored in the m_ConnectedComponentArray
    };

    /// \brief Resets the array of connected components and the ComponentVertex array (which are not valide anymore).
    void resetConnectedComponent();

    /// \brief Returns true iff the array of connected components and the ComponentVertex array are valide (ie : not void)
    bool isvoid_ConnectedComponent();

    /// \brief Computes the array of connected components and the ComponentVertex array (which makes them valide).
    void computeConnectedComponent();

    /// \brief Returns the number of connected components.
    virtual int getNumberOfConnectedComponents();

    /// \brief Returns the FirstVertexIndex of the ith connected component.
    virtual int getFirstVertexIndex(Index i);

    /// \brief Returns the LastVertexIndex of the ith connected component.
    virtual int getLastVertexIndex(Index i);

    /// \brief Returns the size of the ith connected component.
    virtual int getComponentSize(Index i);

    /// \brief Returns the index of the ith connected component.
    virtual int getComponentIndex(Index i);

    /// \brief Returns true iff the ith connected component is closed (ie : iff FirstVertexIndex == LastVertexIndex).
    virtual bool isComponentClosed(Index i);

    /// \brief Returns the indice of the vertex which is next to the vertex indexed by i.
    int getNextVertex(const Index i);

    /// \brief Returns the indice of the vertex which is previous to the vertex indexed by i.
    int getPreviousVertex(const Index i);

    /// \brief Returns the indice of the edge which is next to the edge indexed by i.
    int getNextEdge(const Index i);

    /// \brief Returns the indice of the edge which is previous to the edge indexed by i.
    int getPreviousEdge(const Index i);

    /// \brief Returns the ComponentVertex array.
    const sofa::type::vector< Index > &getComponentVertexArray() const;

    /// \brief Returns the array of connected components.
    const sofa::type::vector< ConnectedComponent > &getConnectedComponentArray() const;

private:
    /** The array that stores for each vertex index, the connected component the vertex belongs to */
    sofa::type::vector< Index > m_ComponentVertexArray;

    /** The array that stores the connected components */
    sofa::type::vector< ConnectedComponent > m_ConnectedComponentArray;
};

} // namespace sofa::component::topology::container::dynamic
