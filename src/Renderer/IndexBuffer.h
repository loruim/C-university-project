#pragma once

#include "glad/glad.h"

/**
* @file IndexBuffer.h
* The file is responsible for Index Buffer
*/

/**
* @namespace RenderEngine Covers all classes responsible for rendering resources
*/
namespace RenderEngine
{

	/**
	* @class IndexBuffer
	* @brief load Index Buffer
	*/
	class IndexBuffer
	{
	public:

		/**
		* A constructor
		*/
		IndexBuffer();

		/**
		* A disconstructor. Removes the index buffer
		*/
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		/** Move initialization
		* @param indexBuffer IndexBuffer&&
		* @return IndexBuffer&
		*/
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		/** move assignment
		* @param indexBuffer IndexBuffer&&
		* @return IndexBuffer&
		*/
		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;

		/**
		* A member
		* @param[in] data void*
		* @param[in] count unsigned int self size
		*/
		void init(const void* data, const unsigned int count);

		/**
		* A const member
		*/
		void bind() const;

		/**
		* A const member
		*/
		void unbind() const;

		/**
		* A const member
		* @return self size
		*/
		unsigned int getCount() const { return m_count; }

	private:
		GLuint m_id;
		unsigned int m_count;
	};
}