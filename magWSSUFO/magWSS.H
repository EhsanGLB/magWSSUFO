/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::magWSS

Group
    grpUtilitiesFunctionObjects

Description
    This function object evaluates and outputs the shear stress at wall
    patches.  The result is written as a volVectorField to time directories as
    field 'magWSS'

        \f[
            Stress = R \dot n
        \f]

    where
    \vartable
        R       | stress tensor
        n       | patch normal vector (into the domain)
    \endvartable

    The shear stress (symmetrical) tensor field is retrieved from the
    turbulence model.  All wall patches are included by default; to restrict
    the calculation to certain patches, use the optional 'patches' entry.

    Example of function object specification:
    \verbatim
    magWSS1
    {
        type        magWSS;
        functionObjectLibs ("libutilityFunctionObjects.so");
        ...
        patches     (".*Wall");
    }
    \endverbatim

    \heading Function object usage
    \table
        Property | Description               | Required    | Default value
        type     | type name: magWSS | yes        |
        patches  | list of patches to process | no         | all wall patches
    \endtable

SourceFiles
    magWSS.C
    IOmagWSS.H

Author
    Ehsan Golab, SUT. All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef magWSS_H
#define magWSS_H

#include "functionObjectFile.H"
#include "volFieldsFwd.H"
#include "Switch.H"
#include "OFstream.H"
#include "pointFieldFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class fvMesh;

/*---------------------------------------------------------------------------*\
                       Class magWSS Declaration
\*---------------------------------------------------------------------------*/

class magWSS
:
    public functionObjectFile
{
protected:

    // Protected data

        //- Name of this set of magWSS object
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Switch to send output to Info as well as to file
        Switch log_;

        //- Optional list of patches to process
        labelHashSet patchSet_;

        //- Name of velocity field, default is "U"
        word UName_;

        //- Name of phi field, default is "phi"
        word phiName_;


    // Protected Member Functions

        //- File header information
        virtual void writeFileHeader(const label i);

        //- Calculate the shear stress
        void calcWSS
        (
            const fvMesh& mesh,
            const volSymmTensorField& Reff,
            volScalarField& WSS
        );

        //- Disallow default bitwise copy construct
        magWSS(const magWSS&);

        //- Disallow default bitwise assignment
        void operator=(const magWSS&);


public:

    //- Runtime type information
    TypeName("magWSS");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        magWSS
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~magWSS();


    // Member Functions

        //- Return name of the set of magWSS
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the magWSS data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the magWSS and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const pointField&)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
