#pragma once
#include "version.hh"

/*!
	* \interface IHelper
	* \brief �����ӿ�
	* \remarks ʵ�ָýӿ��԰�����ɸ�����Ϣ
	*/
struct IHelper
{
	/*!
		* \fn Version GetVersion() const
		* \brief ��ȡ����
		**/
	virtual Version GetVersion() const = 0;

	/*!
		* \fn PrintHelp
		* \brief ��ӡ����
		*/
	virtual void PrintHelp() const = 0;
};