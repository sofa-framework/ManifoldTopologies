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
#include <sofa/component/topology/container/dynamic/TriangleSetGeometryAlgorithms.h>

namespace sofa::component::topology::container::dynamic
{


class ManifoldTriangleSetTopologyContainer;

class ManifoldTriangleSetTopologyModifier;

/**
 * A class that performs topology algorithms on an ManifoldTriangleSet.
 */
template < class DataTypes >
class SOFA_MANIFOLD_TOPOLOGIES_API ManifoldTriangleSetGeometryAlgorithms : public TriangleSetGeometryAlgorithms<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(ManifoldTriangleSetGeometryAlgorithms,DataTypes), SOFA_TEMPLATE(TriangleSetGeometryAlgorithms,DataTypes));

    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Coord Coord;

    using Index = sofa::Index;

    ManifoldTriangleSetGeometryAlgorithms()
        : TriangleSetGeometryAlgorithms<DataTypes>()
        , m_triSwap(initData(&m_triSwap,  "swap 2 triangles by their index", "Debug : Test swap function (only while animate)."))
        , m_swapMesh(initData(&m_swapMesh, false, "Mesh Optimization", "If true, optimize the mesh only by swapping edges"))
    {
    }

    virtual ~ManifoldTriangleSetGeometryAlgorithms()
    {}

    virtual void init() override;

    virtual void reinit() override;


    /** \brief Split triangles to create edges along a path given as a the list of existing edges and triangles crossed by it.
     * Each end of the path is given either by an existing point or a point inside the first/last triangle. If the first/last triangle is (TriangleID)-1, it means that to path crosses the boundary of the surface.
     * @returns the indice of the end point, or -1 if the incision failed.
     */
    virtual int SplitAlongPath(Index pa, Coord& a, Index pb, Coord& b,
            sofa::type::vector< sofa::core::topology::TopologyElementType>& topoPath_list,
            sofa::type::vector<Index>& indices_list,
            sofa::type::vector< sofa::type::Vec<3, double> >& coords_list,
            sofa::type::vector<core::topology::BaseMeshTopology::EdgeID>& new_edges, double epsilonSnapPath = 0.0, double epsilonSnapBorder = 0.0) override;


    /** \brief Duplicates the given edges. Only works if at least the first or last point is adjacent to a border.
     * @returns true if the incision succeeded.
     */
    virtual bool InciseAlongEdgeList(const sofa::type::vector<Index>& edges, sofa::type::vector<Index>& new_points, sofa::type::vector<Index>& end_points, bool& reachBorder) override;


    /** \brief: Swap a list of edges.
     *
     */
    void edgeSwapProcess (const sofa::type::vector <core::topology::BaseMeshTopology::EdgeID>& listEdges);


    /** \brief: Swap the edge adjacent to the two input triangles (To be used by the ray pick interactor).
     *
     */
    void edgeSwapProcess (const core::topology::BaseMeshTopology::TriangleID& indexTri1, const core::topology::BaseMeshTopology::TriangleID& indexTri2);


    /** \brief: Reorder the mesh by swaping a list of edges.
     * For each edge, check if topology will be better before swaping it.
     */
    void swapRemeshing (sofa::type::vector <core::topology::BaseMeshTopology::EdgeID>& listEdges);

    /** \brief: Reorder the whole mesh by swaping a all edges.
     * For each edge, check if topology will be better before swaping it.
     * @see swapRemeshing (const sofa::type::vector <Index>& listedges)
     */
    void swapRemeshing ();

protected:

    Data< sofa::type::vector< Index> > m_triSwap; ///< Debug : Test swap function (only while animate).
    Data< bool > m_swapMesh; ///< If true, optimize the mesh only by swapping edges

    /**\brief Function swaping edge between two adjacents triangles. Create two new triangles and remove the two old one.
    * This function call private functions of the container reordering the different shells.
    * Different from the others used in adding and removing triangles which are faster but need informations of
    * the state's topology before modifications.
    * @see ManifoldTriangleSetTopologyContainer::reorderingTopologyOnROI()
    * @param index of first triangle.
    * @param index of second triangle adjacent to the first one.
    */
    bool edgeSwap (const core::topology::BaseMeshTopology::EdgeID& indexEdge);


private:
    ManifoldTriangleSetTopologyContainer*		              m_container;
    ManifoldTriangleSetTopologyModifier*	                      m_modifier;
    TriangleSetGeometryAlgorithms< DataTypes >*	              m_geometryAlgorithms;
};

#if  !defined(SOFA_MANIFOLD_TOPOLOGY_TRIANGLESETTOPOLOGYALGORITHMS_CPP)
extern template class SOFA_MANIFOLD_TOPOLOGIES_API ManifoldTriangleSetGeometryAlgorithms<sofa::defaulttype::Vec3Types>;
extern template class SOFA_MANIFOLD_TOPOLOGIES_API ManifoldTriangleSetGeometryAlgorithms<sofa::defaulttype::Vec2Types>;
#endif

} // namespace sofa::component::topology::container::dynamic
