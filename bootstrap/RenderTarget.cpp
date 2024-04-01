#include "RenderTarget.h"
#include "gl_core_4_4.h"
#include <vector>

namespace aie {

RenderTarget::RenderTarget()
	: m_width(0),
	m_height(0),
	m_targetCount(0),
	m_targets(nullptr) {
}

RenderTarget::RenderTarget(unsigned int targetCount, unsigned int width, unsigned int height)
	: m_width(0),
	m_height(0),
	m_targetCount(0),
	m_targets(nullptr),
    m_depthTarget(0),
    m_rbo(0){
	initialise(targetCount, width, height);
}

bool RenderTarget::initialise(unsigned int targetCount, unsigned int width, unsigned int height,bool use_depth_texture) {

	//use_depth_texture = false;

	// setup and bind a framebuffer object
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    if (use_depth_texture)
	{
        glGenTextures(1, &m_depthTarget);
        glBindTexture(GL_TEXTURE_2D, m_depthTarget);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        //bind texture to depth map
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTarget, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
	{ // setup and bind a 24bit depth buffer as a render buffer
        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
            width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, m_rbo);
    }
	// create and attach textures
	if (targetCount > 0) {

		m_targets = new Texture[targetCount];

		std::vector<GLenum> drawBuffers = {};

		for (unsigned int i = 0; i < targetCount; ++i) {

			m_targets[i].create(width, height, Texture::RGBA);

			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
								 m_targets[i].getHandle(), 0);
		}

		glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		// cleanup
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		delete[] m_targets;
		m_targets = nullptr;
        if(m_depthTarget)
            glDeleteTextures(1, &m_depthTarget);
        else
		    glDeleteRenderbuffers(1, &m_rbo);
        
		glDeleteFramebuffers(1, &m_fbo);
		m_rbo = 0;
		m_fbo = 0;

		return false;
	}

	// success
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_targetCount = targetCount;
	m_width = width;
	m_height = height;

	return true;
}

RenderTarget::~RenderTarget() {
	delete[] m_targets;
    if (m_depthTarget)
        glDeleteTextures(1, &m_depthTarget);
    else
    	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &m_fbo);
}

void RenderTarget::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void RenderTarget::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::bindRead() const
{
	for (int i = 0; i < m_targetCount; i++) 
	{
		m_targets[i].bind(i);
	}

	// Bind Depth Target
	glActiveTexture(GL_TEXTURE0 + m_targetCount);
	glBindTexture(GL_TEXTURE_2D, m_depthTarget);
}

void RenderTarget::rescaleFrameBuffer(float width, float height)
{
	// Rescale Depth
	if (m_rbo)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_depthTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTarget, 0);
	}

	for (int i = 0; i < m_targetCount; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_targets[i].getHandle());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_targets[i].getHandle(), 0);
	}

	if (width > 0)
		m_width = width;
	if (height > 0)
		m_height = height;

	glViewport(0, 0, m_width, m_height);
}

} // namespace aie