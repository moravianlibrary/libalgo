// Description: Error math zero devision class

// Copyright (c) 2010 - 2013
// Tomas Bayer
// Charles University in Prague, Faculty of Science
// bayertom@natur.cuni.cz

// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.


#ifndef ErrorMathZeroDevision_H
#define ErrorMathZeroDevision_H


#include "ErrorMath.h"


//Math error: devision by zero
template <typename T>
class ErrorMathZeroDevision : public ErrorMath <T>
{
        protected:
                T arg;

        public:
                ErrorMathZeroDevision ( const char * exception_text_, const char * function_text_, const T &arg_ ) :
                        ErrorMath <T> ( exception_text_, function_text_ ), arg ( arg_ ) {};

        public:
                virtual ~ErrorMathZeroDevision() throw() {}

                virtual void printException ( std::ostream * output = &std::cout ) const
                {
                        ErrorMath <T>::printException ( output );
                        *output << arg << '\n';
                }

                virtual T getArg() const {return arg;}
                virtual short getErrorCode() const { return 19;}
};

#endif
