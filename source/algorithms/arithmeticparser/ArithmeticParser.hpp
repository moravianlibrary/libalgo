// Description: Arithmetic parser using postfix notation based on modified Shunting-yard algorithm

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


#ifndef ArithmeticParser_HPP
#define ArithmeticParser_HPP

#include <stack>
#include <cmath>
#include <stdlib.h>
#include <ctype.h>


#include "libalgo/source/const/Const.h"

#include "libalgo/source/exceptions/ErrorMathInvalidArgument.h"
#include "libalgo/source/exceptions/ErrorMathOverflow.h"
#include "libalgo/source/exceptions/ErrorMathZeroDevision.h"
#include "libalgo/source/exceptions/ErrorMathRange.h"
#include "libalgo/source/exceptions/ErrorMathMatrixNotSquare.h"
#include "libalgo/source/exceptions/ErrorParse.h"
#include "libalgo/source/exceptions/Error.h"


template <typename T>
T ArithmeticParser::parseEq ( const char * equation, const T x, const bool print_exception, std::ostream * output )
{
        //Translation equation
        T res = 0;
        char postfix[MAX_TEXT_LENGTH];
        TPlusMinusOperatorTypes plus_minus_types;

        try
        {
                //ArithmeticParser infix notation to postfix notation
                infixToPostfix ( equation, postfix, plus_minus_types );

                //Parse equation
                res = parseEquation ( postfix, plus_minus_types, x, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0 );

                return res;
        }

        //Throw exception
        catch ( ErrorMath <T> & error )
        {
                //Print exeption
                if ( print_exception )
                {
                        error.printException();
                }

                //Throw exception
                throw;
        }

        //Throw exception
        catch ( Error & error )
        {
                if ( print_exception )
                {
                        error.printException ( output );
                }

                throw;
        }
}


template <typename T>
T ArithmeticParser::parseEq ( const char * equation, const T x, const T y, const bool print_exception, std::ostream * output )
{
        //Translation equation
        T res = 0;
        char postfix[MAX_TEXT_LENGTH];
        TPlusMinusOperatorTypes plus_minus_types;

        try
        {
                //ArithmeticParser infix notation to postfix notation
                infixToPostfix ( equation, postfix, plus_minus_types );

                //Parse equation
                res = parseEquation ( postfix, plus_minus_types, x, y, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0, ( T ) 0.0 );

                return res;
        }

        //Throw exception
        catch ( ErrorMath <T> & error )
        {
                //Print exeption
                if ( print_exception )
                {
                        error.printException();
                }

                //Throw exception
                throw;
        }

        //Throw exception
        catch ( Error & error )
        {
                //Print exeption
                if ( print_exception )
                {
                        error.printException ( output );
                }

                //Throw exception
                throw;
        }
}


template <typename T>
T ArithmeticParser::parseEq ( const char * equation,  const T lat, const T lon, const T R, const T a, const T b, const T c, const T lat0, const T lat1, const T lat2, const T theta, const bool print_exception, std::ostream * output )
{
        //Translation cartographic equation
        T res = 0;
        char postfix[MAX_TEXT_LENGTH];
        TPlusMinusOperatorTypes plus_minus_types;

        try
        {
                //ArithmeticParser infix notation to postfix notation
                infixToPostfix ( equation, postfix, plus_minus_types );

                //Parse equation
                res = parseEquation ( postfix, plus_minus_types, ( T ) 0.0 , ( T ) 0.0 , lat, lon, R, a, b, c, lat0, lat1, lat2, theta );

                return res;
        }

        //Throw exception
        catch ( ErrorMath <T> & error )
        {
                //Print exeption
                if ( print_exception )
                {
                        error.printException();
                }

                //Throw exception
                throw;
        }

        //Throw exception
        catch ( Error & error )
        {
                //Print exeption
                if ( print_exception )
                {
                        error.printException();
                }

                //Throw exception
                throw error;
        }
}


template <typename T>
T ArithmeticParser::parseEquation ( const char * equation, const TPlusMinusOperatorTypes & plus_minus_types, const T x, const T y, const T lat, const T lon, const T R, const T a, const T b, const T c, const T lat0, const T lat1, const T lat2, const T theta )

{
        //Parse postfix notation
        unsigned int plus_minus_types_index = 0, n_plus_minus_types = plus_minus_types.size();
        T result = 0.0;

        //Process postfix notation
        std::stack <T> operands;

        while ( *equation != '\0' )
        {
                //NUMBER
                if ( isdigit ( *equation ) )
                {
                        char number_text[32];

                        //Find number and convert to T
                        findSequence ( & equation, number_text );
                        T number = atof ( number_text );

                        //Add into stack
                        operands.push ( number );
                }

                //Function or variable
                else if ( isalpha ( *equation ) )
                {
                        char function_text[32];
                        findSequence ( &equation, function_text );

                        //Functions **************************************************************************************************************

                        //SIN(x)
                        if ( strcmp ( function_text, functs[f_sin] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", "sin(x)" );

                                //Pop one operand
                                T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( abs(op) > MAX_FLOAT ) 
					throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "sin(x), abs(x) > MAX.", op );

                                //Result
                                result = sin ( op * M_PI / 180 );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //COS(x)
                        else if ( strcmp ( function_text, functs[f_cos] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) 
					throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", "cos(x)." );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( fabs(op) > MAX_FLOAT ) 
					throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation: ", "cos(x), abs(x) > MAX.", op );

                                //Result
                                result = cos ( op * M_PI / 180 );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //TAN(x)
                        else if ( ( strcmp ( function_text, functs[f_tg] ) == 0 ) || ( strcmp ( function_text, functs[f_tan] ) == 0 ) )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", "tan(x)" );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( fabs(op) > MAX_FLOAT ) 
					throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "tan(x), abs(x) > MAX.", op );
                                else if ( fabs ( cos ( op * M_PI / 180 ) ) < MIN_FLOAT )
                                {
                                        //throw ErrorMathMatrixNotSquare <T> ( "ERangError: can not parse equation ", "tan(x), x = ", 1.0, 1.0);
                                        throw ErrorMathRange <T> ( "ErrorMathRange: can not parse equation ", "tan(x), x = ", op );
                                }

                                //Result
                                result = tan ( op * M_PI / 180 );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //COT(x)
                        else if ( ( strcmp ( function_text, functs[f_cot] ) == 0 ) || ( strcmp ( function_text, functs[f_cotg] ) == 0 ) )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", "cotg(x)" );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exceptions
                                if ( fabs(op) > MAX_FLOAT )
                                {
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "cotg(x), abs(x) > MAX.", op );
                                }

                                else if ( fabs ( sin ( op * M_PI / 180 ) ) < MIN_FLOAT )
                                {
                                        throw ErrorMathRange <T> ( "ErrorMathRange: can not parse equation ", "cotg(x), x = ", op );
                                }

                                //Result
                                result = 1.0 / tan ( op * M_PI / 180 );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //ASIN(x)
                        else if ( strcmp ( function_text, functs[f_asin] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", "asin(x)" );

                                //Pop one operand
                                T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( abs(op) > MAX_FLOAT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "asin(x), abs(x) > MAX.", op );

                                //Throw exception: fabs (abn_acos) - 1.0 > MIN_FLOAT
                                if ( ( op > 1.0 + ARGUMENT_ROUND_ERROR ) || ( op < - 1.0 - ARGUMENT_ROUND_ERROR ) )
                                {
                                        throw ErrorMathInvalidArgument <T> ( "ErrorMathInvalidArgument: can not parse equation ", "asin(x), x = ", op );
                                }

                                //Correct round errors
                                else if ( op > 1.0 )
                                {
                                        op = 1.0;
                                }

                                else if ( op < - 1.0 )
                                {
                                        op = -1.0;
                                }

                                //Result
                                result = asin ( op ) * 180 / M_PI;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //ACOS(x)
			else if (strcmp(function_text, functs[f_acos]) == 0)
			{
				//Empty stack ?
				if (operands.empty()) throw ErrorParse("ErrorParse: can not parse equation, argument missing: ", "acos(x)");

				//Pop one operand
				T op = operands.top();

				//Remove operand from the stack
				operands.pop();

				//Exception
				if (abs(op) > MAX_FLOAT) throw ErrorMathOverflow <T>("ErrorMathOverflow: can not parse equation ", "acos(x), abs(x) > MAX.", op);

                                //Throw exception: fabs (abn_acos) - 1.0 > MIN_FLOAT
                                if ( ( op > 1.0 + ARGUMENT_ROUND_ERROR ) || ( op < - 1.0 - ARGUMENT_ROUND_ERROR ) )
                                {
                                        throw ErrorMathInvalidArgument <T> ( "ErrorMathInvalidArgument: can not parse equation", "acos(x), x = ", op );
                                }

                                //Correct round errors
                                else if ( op > 1.0 )
                                {
                                        op = 1.0;
                                }

                                else if ( op < - 1.0 )
                                {
                                        op = -1.0;
                                }

                                //Result
                                result = acos ( op ) * 180 / M_PI;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //ATAN(x)
                        else if ( ( strcmp ( function_text, functs[f_atan] ) == 0 ) )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", "atan(x)." );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( fabs(op) > MAX_FLOAT )
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "atan(x), abs(x) > MAX.", op );

                                //Result
                                result = atan ( op ) * 180 / M_PI;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //LN(x)
                        else if ( strcmp ( function_text, functs[f_ln] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, ", "ln(x)" );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( op > MAX_FLOAT )
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "ln(x), x > MAX.", op );
                                else if ( op <= MIN_FLOAT )
                                        throw ErrorMathInvalidArgument <T> ( "ErrorMathInvalidArgument: can not parse equation ", "ln(x), x = ", op );

                                //Result
                                result = log ( op );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //LOG(x)
                        else if ( strcmp ( function_text, functs[f_log] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: ", function_text );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( op > MAX_FLOAT )
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "log(x), x > MAX.", op );
                                else if ( op <= MIN_FLOAT )
                                        throw ErrorMathInvalidArgument <T> ( "ErrorMathInvalidArgument: can not parse equation ", "log(x), x = ", op );

                                //Result
                                result = log10 ( op );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //EXP(x)
                        else if ( strcmp ( function_text, functs[f_exp] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation. argument missing: ", function_text );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( op > MAX_FLOAT_EXPONENT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "exp^x, x > MAX.", op );

                                //Result
                                result = exp ( op );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //SQR(x)
                        else if ( strcmp ( function_text, functs[f_sqr] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", function_text );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( fabs(op) > sqrt ( MAX_FLOAT ) )
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "sqr(x), x > MAX.", op );

                                //Result
                                result = op * op;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //SQRT(x)
                        else if ( strcmp ( function_text, functs[f_sqrt] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing: ", function_text );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( op > MAX_FLOAT )
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "sqrt(x), x > MAX.", op );

                                if ( op < 0 )
                                        throw ErrorMathInvalidArgument <T> ( "ErrorMathInvalidArgument: can not parse equation ", "sqrt(x), x = ", op );

                                //Result
                                result = sqrt ( op );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //ABS(x)
                        else if ( strcmp ( function_text, functs[f_abs] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing:  ", function_text );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( fabs(op) > MAX_FLOAT ) 
					throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "abs(x), x > MAX.", op );

                                //Result
                                result = fabs ( op );

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //sign(x)
                        else if ( strcmp ( function_text, functs[f_sign] ) == 0 )
                        {
                                //Empty stack ?
                                if ( operands.empty() ) throw ErrorParse ( "ErrorParse: can not parse equation, argument missing:  ", function_text );

                                //Pop one operand
                                const T op = operands.top();

                                //Remove operand from the stack
                                operands.pop();

                                //Exception
                                if ( fabs (op) > MAX_FLOAT ) 
					throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "sign(x), fabs(x) > MAX.", op );

                                //Result
                                if ( op > 0 ) result = 1.0;
                                else if ( op < 0 ) result = -1.0;
				else result = 0;
                                

                                //Add result to the stack
                                operands.push ( result );
                        }

                        // Variables **************************************************************************************************************

                        //VARIABLE x
                        else if ( function_text[0] == *vars[v_x] )
                        {
                                //Get result
                                result = x;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE y
                        else if ( function_text[0] == *vars[v_y] )
                        {
                                //Get result
                                result = y;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //CONST RO = PI/ 180
                        else if ( ( strcmp ( function_text, consts[c_RO] )  == 0 ) || ( strcmp ( function_text, consts[c_Ro] ) == 0 ) )
                        {
                                //Get result
                                result = 180 / M_PI;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //CONST PI
                        else if ( ( strcmp ( function_text, consts[c_pi] ) == 0 ) || ( strcmp ( function_text, consts[c_Pi] ) == 0 ) || ( strcmp ( function_text, consts[c_PI] ) == 0 ) )
                        {
                                //Get result
                                result = M_PI;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE R
                        else if ( function_text[0] == *vars[v_R] )
                        {
                                //Get result
                                result = R;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE a
                        else if ( function_text[0] == *vars[v_a] )
                        {
                                //Get result
                                result = a;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE b
                        else if ( function_text[0] == *vars[v_b] )
                        {
                                //Get result
                                result = a;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE c
                        else if ( function_text[0] == *vars[v_c] )
                        {
                                //Get result
                                result = c;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE lon
                        else if ( ( strcmp ( function_text, vars[v_lon] ) == 0 ) || ( strcmp ( function_text, vars[v_lam] )  == 0 ) || ( strcmp ( function_text, vars[v_v] ) == 0 ) )
                        {
                                //Get result
                                result = lon;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE lat
                        else if ( ( strcmp ( function_text, vars[v_lat] ) == 0 ) || ( strcmp ( function_text, vars[v_phi] ) == 0 ) || ( strcmp ( function_text, vars[v_u] ) == 0 ) )
                        {
                                //Get result
                                result = lat;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE lat0
                        else if ( ( strcmp ( function_text, vars[v_lat0] ) == 0 ) || ( strcmp ( function_text, vars[v_phi0] ) == 0 ) || ( strcmp ( function_text, vars[v_u0] ) == 0 ) )
                        {
                                //Get result
                                result = lat0;

                                //Add result to the stack
                                operands.push ( result );
                        }

			
                        //VARIABLE lat1
			else if ((strcmp(function_text, vars[v_lat1]) == 0) || (strcmp(function_text, vars[v_phi1]) == 0) || (strcmp(function_text, vars[v_u1]) == 0))
                        {
                                //Get result
                                result = lat1;

                                //Add result to the stack
                                operands.push ( result );
                        }

                        //VARIABLE lat2
			else if ((strcmp(function_text, vars[v_lat2]) == 0) || (strcmp(function_text, vars[v_phi2]) == 0) || (strcmp(function_text, vars[v_u2]) == 0))
                        {
                                //Get result
                                result = lat2;

                                //Add result to the stack
                                operands.push ( result );
                        }
			
			//VARIABLE theta
			else if (strcmp(function_text, vars[v_theta]) == 0)
			{
				//Get result
				result = theta;

				//Add result to the stack
				operands.push(result);
			}

                        //Unknown variable
                        else
                        {
                                //Throw exception
                                throw ErrorParse ( "ErrorParse: can not parse equation, unknown variable ", function_text );
                        }
                }

                //OPERATORS **************************************************************************************************************

                //Operators ^ * / + - (sorted according the priority)
                else if ( ( *equation == '^' ) || ( *equation == '*' ) || ( *equation == '/' ) || ( *equation == '+' ) || ( *equation == '-' ) )
                {
                        //Empty stack ?
                        if ( operands.empty() ) throw ErrorParse ( "ErrorParse: ", "Invalid second argument for operation +, -, *, /." );

                        //Pop the second operand
                        const  T op2 = operands.top();

                        //Remove operand from the stack
                        operands.pop();

                        //Process the second operand only if there is a binary operator
                        T op1 =  0.0 ;

                        if ( ( *equation == '*' )  || ( *equation == '/' ) || ( *equation == '^' ) || ( ( *equation == '+' ) || ( *equation == '-' ) ) &&
                                        ( plus_minus_types_index < n_plus_minus_types ) && ( plus_minus_types[plus_minus_types_index] == BinaryOperator ) )
                        {
                                //Pop the first operand
                                op1 = operands.top();

                                //Remove operand from the stack
                                operands.pop();
                        }

                        //Compute result (Power)
                        if ( *equation == '^' )
                        {
                                //Convergate to zero
                                if ( op2 < -MAX_FLOAT_EXPONENT )  result = 0;

                                //Overflows
                                if ( op2 > MAX_FLOAT_EXPONENT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "x^y, number > MAX", op2 );

                                if ( op1 > MAX_FLOAT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "x^y, exponent > MAX.", op1 );

                                result = pow ( op1, op2 );
                        }

                        //Compute result (Multiply)
                        else if ( *equation == '*' )
                        {
                                //Exception
                                if ( op2 > MAX_FLOAT || op1 > MAX_FLOAT )
                                {
                                        throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "x * y, number > MAX", op2 );
                                }

                                result = op1 * op2;
                        }

                        //Compute result (Divide)
                        else if ( *equation == '/' )
                        {
                                //Exception
                                if ( fabs ( op2 ) < MIN_FLOAT )
                                {
                                        throw  ErrorMathZeroDevision <T> ( "ErrorMathDivisonByZero: can not parse equation ", "x / y, y = 0.", op2 );
                                }

                                result = op1 / op2;
                        }

                        //Compute result (Add)
                        else if ( *equation == '+' )
                        {
                                //Exception
                                if ( fabs ( op2 ) + fabs ( op1 ) > MAX_FLOAT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "x + y, result > MAX", op2 );

                                //Increment plus-minus index
                                plus_minus_types_index++;

                                result = op1 + op2;
                        }

                        //Compute result (Subtract)
                        else if ( *equation == '-' )
                        {
                                //Exception
                                if ( fabs ( op2 ) > MAX_FLOAT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "x - y, number > MAX", op2 );

                                if ( fabs ( op1 ) > MAX_FLOAT ) throw ErrorMathOverflow <T> ( "ErrorMathOverflow: can not parse equation ", "x - y, number > MAX", op1 );

                                //Increment plus-minus index
                                plus_minus_types_index++;

                                result = op1 - op2;
                        }

                        //Add result to the stack
                        operands.push ( result );

                        //Jump to next char
                        equation ++;
                }

                //Space, TAB
                else if ( ( *equation == ' ' ) || ( *equation == '\t' ) )
                {
                        equation ++;
                }

                //Illegal character
                else
                {
                        //Error
                        throw ErrorParse ( "ErrorParse: ", "Illegal character in equation, parsing stopped." );
                }
        }

        //Get result from the stack
        if ( !operands.empty() )
        {
                //Get the result
                result = operands.top();

                //Remove result from the stack
                operands.pop();

                //Is this result correct?
                if ( operands.empty() )
                {
                        //Result is correct
                        return result;
                }

                //Something in the stack
                else
                {
                        //Error in equation
                        char oper[64];
                        sprintf ( oper, "%20.4f", operands.top() );

                        throw ErrorParse ( "ErrorParse: can not parse equation, bad argument: ", oper );
                }
        }

        //Empty stack before result calculation
        else
        {
                throw ErrorParse ( "ErrorParse: can not parse equation, ", " no equation." );
        }
}

#endif
