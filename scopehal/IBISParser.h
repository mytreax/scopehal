/***********************************************************************************************************************
*                                                                                                                      *
* ANTIKERNEL v0.1                                                                                                      *
*                                                                                                                      *
* Copyright (c) 2012-2020 Andrew D. Zonenberg                                                                          *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

/**
	@file
	@author Andrew D. Zonenberg
	@brief Declaration of IBISParser and related classes
 */

#ifndef IBISParser_h
#define IBISParser_h

/**
	@brief A single current/voltage point
 */
class IVPoint
{
public:
	IVPoint()
	{}

	IVPoint(float v, float i)
	: m_voltage(v)
	, m_current(i)
	{}

	float m_voltage;
	float m_current;
};

/**
	@brief A generic current/voltage curve
 */
class IVCurve
{
public:

	float InterpolateCurrent(float voltage);

	///@brief The raw I/V curve data
	std::vector<IVPoint> m_curve;
};

/**
	@brief An IBIS model (for a single type of buffer)

	For now, we only support I/O or output type models and ignore all inputs.
 */
class IBISModel
{
public:
	IBISModel(std::string name)
	: m_type(TYPE_IO)
	, m_name(name)
	{}

	//Model type
	enum type_t
	{
		TYPE_INPUT,
		TYPE_IO,
		TYPE_OPEN_DRAIN,
		TYPE_OUTPUT,
		TYPE_SERIES,
		TYPE_TERMINATOR
	} m_type;

	//Name of the model
	std::string	m_name;

	enum corner_t
	{
		CORNER_MIN,
		CORNER_TYP,
		CORNER_MAX
	};

	//I/V curves for each output buffer (indexed by corner)
	IVCurve m_pulldown[3];
	IVCurve m_pullup[3];

	//Input thresholds (indexed by corner)
	float m_vil[3];
	float m_vih[3];

	//Temperature and voltage values at each corner
	float m_temps[3];
	float m_voltages[3];

	//For now, ignore waveforms
};

/**
	@brief IBIS file parser (may contain multiple models)
 */
class IBISParser
{
public:
	IBISParser();
	virtual ~IBISParser();

	void Clear();
	bool Load(std::string fname);

	std::string m_component;
	std::string m_manufacturer;

	std::map<std::string, IBISModel*> m_models;
};

#endif
