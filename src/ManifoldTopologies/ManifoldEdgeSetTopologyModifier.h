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
#include <sofa/component/topology/container/dynamic/EdgeSetTopologyModifier.h>

namespace sofa::component::topology::container::dynamic
{
class ManifoldEdgeSetTopologyContainer;

using core::topology::BaseMeshTopology;
typedef BaseMeshTopology::EdgeID EdgeID;
typedef BaseMeshTopology::Edge Edge;
typedef BaseMeshTopology::SeqEdges SeqEdges;
typedef BaseMeshTopology::EdgesAroundVertex EdgesAroundVertex;

/**
* A class that can apply basic transformations on a set of points.
*/
class SOFA_MANIFOLD_TOPOLOGIES_API ManifoldEdgeSetTopologyModifier : public EdgeSetTopologyModifier
{
public:
    SOFA_CLASS(ManifoldEdgeSetTopologyModifier,EdgeSetTopologyModifier);

    ManifoldEdgeSetTopologyModifier()
        : EdgeSetTopologyModifier()
    {}

    ~ManifoldEdgeSetTopologyModifier() override {}

    void init() override;

    /** \brief Add an edge.
    */
    void addEdgeProcess(Edge e);

    using EdgeSetTopologyModifier::removeEdges;
    /** \brief Remove a set  of edges
    @param edges an array of edge indices to be removed (note that the array is not const since it needs to be sorted)
    *
    */
    // side effect: edges are sorted in removeEdgesWarning
    virtual void removeEdges(/*const*/ sofa::type::vector< Index >& edges,
            const bool removeIsolatedPoints = true);

    using EdgeSetTopologyModifier::removeItems;
    /** \brief Generic method to remove a list of items.
     */
    virtual void removeItems(/*const*/ sofa::type::vector< EdgeID >& items);

    /** \brief add a set  of edges
    @param edges an array of pair of vertex indices describing the edge to be created
    *
    */
    virtual void addEdges(const sofa::type::vector< Edge >& edges) override;

    /** \brief add a set  of edges
    @param edges an array of pair of vertex indices describing the edge to be created
    @param ancestors for each edge to be created provides an array of edge ancestors (optional)
    @param baryCoefs for each edge provides the barycentric coordinates (sum to 1) associated with each ancestor (optional)
    *
    */
    virtual void addEdges(const sofa::type::vector< Edge >& edges,
            const sofa::type::vector< sofa::type::vector< Index > > & ancestors,
            const sofa::type::vector< sofa::type::vector< double > >& baryCoefs) override;

    /** \brief Swap a list of pair edges, replacing each edge pair ((p11, p12), (p21, p22)) by the edge pair ((p11, p21), (p12, p22))
    *
    */
    virtual void swapEdges(const sofa::type::vector< sofa::type::vector< Index > >& edgesPairs) override;

    /** \brief Fuse a list of pair edges, replacing each edge pair ((p11, p12), (p21, p22)) by one edge (p11, p22)
    *
    * @param removeIsolatedPoints if true isolated vertices are also removed
    */
    virtual void fuseEdges(const sofa::type::vector< sofa::type::vector< Index > >& edgesPairs, const bool removeIsolatedPoints = true) override;

    /** \brief Split an array of edges, replacing each edge (p1, p2) by two edges (p1, p3) and (p3, p2) where p3 is the new vertex
    * On each edge, a vertex is created based on its barycentric coordinates
    *
    * @param removeIsolatedPoints if true isolated vertices are also removed
    */
    virtual void splitEdges( sofa::type::vector<Index> &indices,
            const bool removeIsolatedPoints = true) override;

    /** \brief Split an array of edges, replacing each edge (p1, p2) by two edges (p1, p3) and (p3, p2) where p3 is the new vertex
    * On each edge, a vertex is created based on its barycentric coordinates
    *
    * @param removeIsolatedPoints if true isolated vertices are also removed
    */
    virtual void splitEdges( sofa::type::vector<Index> &indices,
            const sofa::type::vector< sofa::type::vector< double > >& baryCoefs,
            const bool removeIsolatedPoints = true) override;
protected:
    /** \brief Add some edges to this topology.
    *
    * \sa addEdgesWarning
    */
    virtual void addEdgesProcess(const sofa::type::vector< Edge >& edges) override;

    /** \brief Effectively Remove a subset of edges. Eventually remove isolated vertices
    *
    * Elements corresponding to these edges are removed form the mechanical object's state vectors.
    *
    * Important : some structures might need to be warned BEFORE the edges are actually deleted, so always use method removeEdgesWarning before calling removeEdgesProcess.
    * \sa removeEdgesWarning
    *
    * Important : parameter indices is not const because it is actually sorted from the highest index to the lowest one.
    *
    * @param removeIsolatedItems if true isolated vertices are also removed
    */
    virtual void removeEdgesProcess(const sofa::type::vector<Index>& indices, const bool removeIsolatedItems = false) override;


private:
    ManifoldEdgeSetTopologyContainer* 	m_container;
};

} // namespace sofa::component::topology::container::dynamic
