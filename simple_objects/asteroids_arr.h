#ifndef ASTEROIDS_ARR
#define ASTEROIDS_ARR
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaider.h"
#include "camera.h"
#include "model.h"

#include <iostream>
class asteroid
{
private:
	unsigned int amount;
	glm::mat4* modelMatrices;
	float radius;
	float offset;
public:
	asteroid(Model rock, unsigned int am, float R, float Oset)
	{
		radius = R;
		offset = Oset;
		amount = am;
		modelMatrices = new glm::mat4[amount];
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius;
			float y = 0.4f; 
			float displacementt = (rand() % (int)(2 * 2 * 100)) / 100.0f - 1;
			float z = cos(angle) * radius;
			model = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			model = glm::translate(model, glm::vec3(radius + displacement, 0.f + displacementt, 0.f));
			model = glm::translate(model, glm::vec3(x, y, z));
			float scale = (rand() % 1) / 100.0f + 0.08;
			model = glm::scale(model, glm::vec3(scale));
			modelMatrices[i] = model;
		}
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		for (unsigned int i = 0; i < rock.meshes.size(); i++)
		{
			unsigned int VAO = rock.meshes[i].VAO;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}
	void Draw(Model rock) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
		for (unsigned int i = 0; i < rock.meshes.size(); i++)
		{
			glBindVertexArray(rock.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}
	}
	~asteroid() {
	}
};
#endif