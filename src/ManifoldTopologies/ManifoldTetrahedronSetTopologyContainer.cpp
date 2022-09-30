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

#include <ManifoldTopologies/ManifoldTetrahedronSetTopologyContainer.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa::component::topology::container::dynamic
{

using namespace std;
using namespace sofa::defaulttype;

int ManifoldTetrahedronSetTopologyContainerClass = core::RegisterObject("Manifold Tetrahedron set topology container")
        .add< ManifoldTetrahedronSetTopologyContainer >()
        ;

//const unsigned int edgesInTetrahedronArray[6][2] = {{0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3}};

ManifoldTetrahedronSetTopologyContainer::ManifoldTetrahedronSetTopologyContainer()
    : TetrahedronSetTopologyContainer()// draw to be restored
    , debugViewIndices( initData(&debugViewIndices, (bool) false, "debugViewTriangleIndices", "Debug : view triangles indices") )
    , debugViewIndicesTetra( initData(&debugViewIndicesTetra, (bool) false, "debugViewTetraIndices", "Debug : view tetra indices") )
    , shellDisplay( initData(&shellDisplay, (bool) false, "debugViewShells", "Debug : view shells tetra"))
{
}


void ManifoldTetrahedronSetTopologyContainer::reinit()
{
    //	int test;

    /*	Tetrahedron tetra_test=Tetrahedron(m_tetrahedron[1][0], m_tetrahedron[1][3], m_tetrahedron[1][2], m_tetrahedron[1][1]);
     test = getTetrahedronOrientation(m_tetrahedron[1], tetra_test);
     msg_info() << "res: " << test;

     Tetrahedron tetra_test2=Tetrahedron(m_tetrahedron[1][2], m_tetrahedron[1][3], m_tetrahedron[1][0], m_tetrahedron[1][1]);
     test = getTetrahedronOrientation(m_tetrahedron[1], tetra_test2);
     msg_info() << "res: " << test;
     */


    createTetrahedraAroundEdgeArray();
    createTetrahedraAroundTriangleArray();


    /*		for (unsigned int i = 0 ; i <m_edge.size();i++)
     msg_info() << i  << " => " << m_edge[i] ;

     for (unsigned int i = 0; i < m_tetrahedraAroundEdge.size(); i++)
     msg_info() << i << " => " << m_tetrahedraAroundEdge[i];

     for (unsigned int i =0; i<m_tetrahedron.size();i++)
     msg_info() << i << " => "<<m_tetrahedron[i];
     */
}


void ManifoldTetrahedronSetTopologyContainer::init()
{
    TetrahedronSetTopologyContainer::init();
}



void ManifoldTetrahedronSetTopologyContainer::createTetrahedraAroundVertexArray ()
{
    // TO be implemented
    // see late: for the topology, only one connexe composante around one vertex.

    TetrahedronSetTopologyContainer::createTetrahedraAroundVertexArray();
}



void ManifoldTetrahedronSetTopologyContainer::createTetrahedraAroundEdgeArray ()
{
    // Get edge array
    sofa::type::vector<Edge> edges = getEdgeArray();

    // Creating Tetrahedrons edges shell unordered
    TetrahedronSetTopologyContainer::createTetrahedraAroundEdgeArray();
    helper::ReadAccessor< Data< sofa::type::vector<Tetrahedron> > > m_tetrahedron = d_tetrahedron;
    //	for (unsigned int i = 0; i < m_tetrahedraAroundEdge.size(); i++)
    //  msg_info() << i << " => " << m_tetrahedraAroundEdge[i];


    for (unsigned int edgeIndex =0; edgeIndex<edges.size(); edgeIndex++)
    {

        auto &shell = getTetrahedraAroundEdgeForModification (edgeIndex);
        sofa::type::vector < sofa::type::vector <Index> > vertexTofind;
        sofa::type::vector <Index> goodShell;
        unsigned int firstVertex =0;
        unsigned int secondVertex =0;
        unsigned int cpt = 0;

        vertexTofind.resize (shell.size());

        // Path to follow creation
        for (unsigned int tetraIndex = 0; tetraIndex < shell.size(); tetraIndex++)
        {
            cpt = 0;

            for (unsigned int vertex = 0; vertex < 4; vertex++)
            {
                if(m_tetrahedron[shell[ tetraIndex]][vertex] != edges[edgeIndex][0] && m_tetrahedron[shell[ tetraIndex]][vertex] != edges[edgeIndex][1] )
                {
                    vertexTofind[tetraIndex].push_back (m_tetrahedron[shell[ tetraIndex]][vertex]);
                    cpt++;
                }

                if (cpt == 2)
                    break;
            }
        }

        Tetrahedron tetra_first = Tetrahedron(edges[edgeIndex][0], edges[edgeIndex][1], vertexTofind[0][0], vertexTofind[0][1]);

        int good = getTetrahedronOrientation (m_tetrahedron[shell[ 0]], tetra_first);

        if (good == 1) //then tetra is in good order, initialisation.
        {
            firstVertex = vertexTofind[0][0];
            secondVertex = vertexTofind[0][1];
        }
        else if (good == 0)
        {
            firstVertex = vertexTofind[0][1];
            secondVertex = vertexTofind[0][0];
        }
        else
        {
            msg_error() << "CreateTetrahedraAroundEdgeArray: Houston there is a problem." ;
        }

        goodShell.push_back(shell[0]);

        bool testFind = false;
        bool reverse = false;
        cpt = 0;


        // Start following path
        for (unsigned int i = 1; i < shell.size(); i++)
        {
            for (unsigned int j = 1; j < shell.size(); j++)
            {

                Tetrahedron tetra_test1 = Tetrahedron(edges[edgeIndex][0], edges[edgeIndex][1], secondVertex, vertexTofind[j][1]);
                Tetrahedron tetra_test2 = Tetrahedron(edges[edgeIndex][0], edges[edgeIndex][1], secondVertex, vertexTofind[j][0]);

                if (vertexTofind[j][0] == secondVertex && getTetrahedronOrientation (m_tetrahedron[shell[ j]], tetra_test1) == 1) //find next tetra, in one or the other order.
                {
                    goodShell.push_back(shell[j]);
                    secondVertex = vertexTofind[j][1];
                    testFind = true;
                    break;
                }
                else if(vertexTofind[j][1] == secondVertex && getTetrahedronOrientation (m_tetrahedron[shell[ j]], tetra_test2) == 1)
                {
                    goodShell.push_back(shell[j]);
                    secondVertex = vertexTofind[j][0];
                    testFind = true;
                    break;
                }
            }

            if (!testFind) //tetra has not be found, this mean we reach a border, we reverse the method
            {
                reverse = true;
                break;
            }

            cpt++;
            testFind =false;
        }


        // Reverse path following methode
        if(reverse)
        {
            for (unsigned int i = cpt+1; i<shell.size(); i++)
            {
                for (unsigned int j = 0; j<shell.size(); j++)
                {

                    Tetrahedron tetra_test1 = Tetrahedron(edges[edgeIndex][0], edges[edgeIndex][1], vertexTofind[j][1], firstVertex);
                    Tetrahedron tetra_test2 = Tetrahedron(edges[edgeIndex][0], edges[edgeIndex][1], vertexTofind[j][0], firstVertex);

                    if (vertexTofind[j][0] == firstVertex && getTetrahedronOrientation (m_tetrahedron[shell[ j]], tetra_test1) == 1) //find next tetra, in one or the other order.
                    {
                        goodShell.insert (goodShell.begin(),shell[j]);
                        firstVertex = vertexTofind[j][1];
                        testFind = true;
                        break;
                    }
                    else if(vertexTofind[j][1] == firstVertex && getTetrahedronOrientation (m_tetrahedron[shell[ j]], tetra_test2) == 1)
                    {
                        goodShell.insert (goodShell.begin(),shell[j]);
                        firstVertex = vertexTofind[j][0];
                        testFind = true;
                        break;
                    }
                }
            }
        }

        shell = goodShell;
        goodShell.clear();
        vertexTofind.clear();
    }
}


void ManifoldTetrahedronSetTopologyContainer::createTetrahedraAroundTriangleArray ()
{
    //msg_info() << "ManifoldTetrahedronSetTopologyContainer::createTetrahedraAroundTriangleArray ()";
    // To be implemented
    // at most 2 tetrahedrons adjacent to one triangle.

    TetrahedronSetTopologyContainer::createTetrahedraAroundTriangleArray();
    helper::ReadAccessor< Data< sofa::type::vector<Tetrahedron> > > m_tetrahedron = d_tetrahedron;
    helper::ReadAccessor< Data< sofa::type::vector<Triangle> > > m_triangle = d_triangle;
    //	for (unsigned int i = 0; i <m_tetrahedraAroundTriangle.size();i++)
    // msg_info() << i << " old => " << m_tetrahedraAroundTriangle[i];


    for (unsigned int triangleIndex = 0; triangleIndex < m_tetrahedraAroundTriangle.size(); triangleIndex++)
    {
        auto &shell = getTetrahedraAroundTriangleForModification (triangleIndex);

        if (shell.size() == 1)
        {
            //Check if triangle has the good orientation
            int test = getTriangleTetrahedronOrientation (m_tetrahedron[ shell[0] ], m_triangle[ triangleIndex ]);
            msg_info() << "Border test: " << test;
        }
        else if (shell.size() == 2)
        {
            int test = getTriangleTetrahedronOrientation (m_tetrahedron[ shell[0] ], m_triangle[ triangleIndex ]);
            if ( test == 0) // not in good order in first tetra of the shell, need to swap
            {
                unsigned int buffer = shell[0];
                shell[0] = shell[1];
                shell[1] = buffer;
            }
        }
        else
        {
            msg_error() << "CreateTetrahedraAroundTriangleArray, manifold topology is not fullfil.";
        }
    }

    //	for (unsigned int i = 0; i <m_tetrahedraAroundTriangle.size();i++)
    //	  msg_info() << i << " new => " << m_tetrahedraAroundTriangle[i];

}


bool ManifoldTetrahedronSetTopologyContainer::checkTopology() const
{
    bool ret = true;

    // To be implemented later later....

    return ret && TetrahedronSetTopologyContainer::checkTopology();
}



void ManifoldTetrahedronSetTopologyContainer::clear()
{
    //To be completed if necessary
    TetrahedronSetTopologyContainer::clear();
}


int ManifoldTetrahedronSetTopologyContainer::getTetrahedronOrientation (const Tetrahedron &t_ref, const Tetrahedron &t_test )
{
    std::map<unsigned int, unsigned int> mapPosition;
    unsigned int positionsChange[4];
    std::map<unsigned int, unsigned int>::iterator it;

    unsigned int permutation=0;
    unsigned int buffer;

    for (unsigned int i = 0; i< 4; i++)
    {
        mapPosition[t_ref[i]] = i;
    }

    for (unsigned int i= 0; i <4; i++)
    {
        it = mapPosition.find (t_test[i]);
        if (it == mapPosition.end())
        {
            msg_error() << "GetTetrahedronOrientation: reference and testing tetrahedrons are not composed by the same vertices.";
            return -1;
        }
        positionsChange[(*it).second] = i;
    }

    for (unsigned int i = 0; i <4; i++)
    {
        if( positionsChange[i] != i)
        {
            for (unsigned int j= i; j<4; j++)
            {
                if(positionsChange[j]==i)
                {
                    buffer = positionsChange[i];
                    positionsChange[i] = positionsChange[j];
                    positionsChange[j] = buffer;
                    permutation++;
                    break;
                }
            }
        }
    }

    if( permutation%2 == 0)
        return 1;
    else
        return 0;
}
int ManifoldTetrahedronSetTopologyContainer::getEdgeTriangleOrientation(const Triangle& f, const Edge& e)
{
    unsigned i = 0;
    for(; i < 3; ++i)
    {
        if(e[0] == f[i] && e[1] == f[(i+1)%3])
            return 1;
        if(e[0] == f[i] && e[1] == f[(i+2)%3])
            return -1;
    }
    return 0;
}

int ManifoldTetrahedronSetTopologyContainer::getTriangleTetrahedronOrientation (const Tetrahedron &t, const Triangle &tri )
{
    //msg_info() << "ManifoldTetrahedronSetTopologyContainer::getTriangleTetrahedronOrientation ()";
    //To be done in better way:

    std::map <unsigned int, unsigned int> mapPosition;
    std::map<unsigned int, unsigned int>::iterator it;
    unsigned int positionsChange[3];
    sofa::type::vector <Triangle> positifs;



    for (unsigned int i = 0; i< 4; i++)
    {
        mapPosition[t[i]] = i;
    }

    for (unsigned int i = 0; i<3; i++)
    {
        it = mapPosition.find (tri[i]);
        if (it == mapPosition.end())
        {
            msg_error() << "GetTriangleTetrahedronOrientation: tetrahedrons and triangle are not composed by the same vertices.";
            return -1;
        }
        positionsChange[i] = (*it).second;
    }


    // a la barbare:

    Triangle triBuf;

    triBuf = Triangle (1,2,3);  positifs.push_back (triBuf);
    triBuf = Triangle (2,3,1);  positifs.push_back (triBuf);
    triBuf = Triangle (3,1,2);  positifs.push_back (triBuf);

    triBuf = Triangle (3,2,0);  positifs.push_back (triBuf);
    triBuf = Triangle (2,0,3);  positifs.push_back (triBuf);
    triBuf = Triangle (0,3,2);  positifs.push_back (triBuf);

    triBuf = Triangle (0,1,3);  positifs.push_back (triBuf);
    triBuf = Triangle (1,3,0);  positifs.push_back (triBuf);
    triBuf = Triangle (3,0,1);  positifs.push_back (triBuf);

    triBuf = Triangle (2,1,0);  positifs.push_back (triBuf);
    triBuf = Triangle (1,0,2);  positifs.push_back (triBuf);
    triBuf = Triangle (0,2,1);  positifs.push_back (triBuf);

    for (unsigned int j =0; j<positifs.size(); j++)
    {
        if ( positionsChange[0] == positifs[j][0] && positionsChange[1] == positifs[j][1] && positionsChange[2] == positifs[j][2])
        {
            return 1;
        }
    }

    return 0;
}


/*

 - equivalent to TrianglesAroundEdge [i]
 - first triangle of the tetrahedron should be in positive orientation
 - This first triangle is the one on the border if tetrahedron is on border.
 - return either negatif or positive orientation in the tetrahedron or -1 if error.

 => should be used in createTetrahedraAroundTriangleArray



 for(TetraID i = 0; i < m_nbTetrahedra; ++i)
 {
 const Tetra& t = m_topo->getTetrahedron(i);
 const TrianglesInTetrahedron& tFaces = m_topo->getTrianglesInTetrahedron(i);
 for(int l = 0; l < 4; ++l)
 {
 int sign = 1;
 const Triangle& f = m_topo->getTriangle(tFaces[l]);

 int m = 0;
 while(t[m] == f[0] || t[m] == f[1] || t[m] == f[2])
 ++m;
 if(m%2 == 1)
 sign *= -1;

 int n = 0;
 while(f[0] != t[n])
 ++n;

 if((n+1)%4 == m && f[2] == t[(n+2)%4])
 sign *= -1;
 if((n+1)%4 != m && f[2] == t[(n+1)%4])
 sign *= -1;
 }
 }
 */

//no warnings:

} // namespace sofa::component::topology::container::dynamic

