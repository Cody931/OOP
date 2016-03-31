#include "stdafx.h"
#include "Calculator.h"


CCalculator::CCalculator()
{
}

bool CCalculator::CheckVar(const std::string & varName)
{
	auto it = m_vars.find(varName);
	if (it != m_vars.end())
	{
		return true;
	}
	return false;
}

bool CCalculator::CheckFn(const std::string & varName)
{
	auto it = m_functions.find(varName);
	if (it != m_functions.end())
	{
		return true;
	}
	return false;
}

GetValInfo CCalculator::GetVar(const std::string & varName)
{
	GetValInfo info;
	if (CheckVar(varName))
	{
		info.wasError = GetError::valueIsFound;
		info.value = m_vars[varName];
	}
	else
	{
		info.wasError = GetError::noValue;
	}
	return info;
}

bool CCalculator::SetVar(const std::string & varName)
{
	if (!CheckVar(varName))
	{
		m_vars.insert(std::pair<std::string, double>(varName, NAN));
		return true;
	}
	return false;
}

bool CCalculator::SetLetDouble(const std::string & varName, const double & value)
{
	if (!CheckFn(varName))
	{
		m_vars[varName] = value;
		return true;
	}
	return false;
}

bool CCalculator::SetLetVar(const std::string & varName, const std::string & valueStr)
{
	GetValInfo valueInfo = GetVar(valueStr);
	if (valueInfo.wasError == GetError::noValue)
	{
		double value = atof(valueStr.c_str());
		if (valueStr != "0" && value == 0)
		{
			return false;
		}
		return SetLetDouble(varName, value);
	}
	else if (!CheckFn(varName))
	{
		m_vars[varName] = valueInfo.value;
		return true;
	}
	return false;
}

double CCalculator::CalcFunctions(const double & firstValue, const OperandType & operand, const double & secondValue)
{
	if (operand == OperandType::Add)
	{
		return (firstValue + secondValue);
	}
	else if (operand == OperandType::Sub)
	{
		return (firstValue - secondValue);
	}
	else if (operand == OperandType::Div)
	{
		return (firstValue / secondValue);
	}
	else if (operand == OperandType::Mul)
	{
		return (firstValue * secondValue);
	}
	return 0;
}

GetValInfo CCalculator::GetFn(const std::string & fnName)
{
	GetValInfo infoResult;
	GetFnInfo infoFn;
	auto it = m_functions.find(fnName);
	if (it != m_functions.end())
	{
		infoResult.wasError = GetError::valueIsFound;
		infoFn = it->second;

		auto firstVal = GetVar(infoFn.firstVal);
		auto operand = infoFn.operand;
		auto secondVal = GetVar(infoFn.secondVal);
		if (firstVal.wasError == GetError::noValue)
		{
			firstVal = GetFn(infoFn.firstVal);
		}
		if (secondVal.wasError == GetError::noValue)
		{
			secondVal = GetFn(infoFn.secondVal);
		}

		infoResult.value = CalcFunctions(firstVal.value, operand, secondVal.value);
	}
	else
	{
		infoResult.wasError = GetError::noValue;
	}
	return infoResult;
}


bool CCalculator::SetFnOperand(const std::string & fnName, const std::string & firstValue, OperandType operand, const std::string & secondValue)
{
	GetFnInfo info;
	if ((CheckVar(firstValue) || CheckFn(firstValue)) && (CheckVar(secondValue) || CheckFn(secondValue)))
	{
		info.firstVal = firstValue;
		info.operand = operand;
		info.secondVal = secondValue;
		m_functions.insert(std::pair<std::string, GetFnInfo>(fnName, info));
		return false;
	}
	return true;
}
