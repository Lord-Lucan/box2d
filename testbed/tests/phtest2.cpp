// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <list>
#include <chrono>
#include <cmath>

#include "test.h"

class PHTest2 : public Test
{
	const int m_boxCount=60;
	std::list<b2Body*> m_boxes;
	time_t m_time=0;
    const double m_step = (2.0*b2_pi)/8.0;
    int m_angleSteps = -2.0;
    const int m_x = 32;
	const int m_y = 18;

    b2Vec2 getGravityVector() const {
        return getGravityVector(double(m_angleSteps)*m_step);
	}

    b2Vec2 getGravityVector(const double angle) const {
        const double x = 10.0*std::cos(angle);
        const double y = 10.0*std::sin(angle);
		return b2Vec2(x,y);
	}

    b2Vec2 getSourcePosition(const double angle) const {
        const double x = 10.0*std::cos(angle);
        const double y = 10.0*std::sin(angle);
		return b2Vec2(x,y);
	}

public:

	PHTest2()
	{
		m_world->SetGravity(getGravityVector());
		{
			// Create the body definition and set body properties.
			b2BodyDef boxDef;
			boxDef.position.Set(0.0f, 20.0f);

			// Actually create the body in the world.
			b2Body* boxBody = m_world->CreateBody(&boxDef);

			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 18.0f, b2Vec2( 32.0f, 0.0f), 0.0);
			boxBody->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(0.5f, 18.0f, b2Vec2(-32.0f, 0.0f), 0.0);
			boxBody->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(32.0f, 0.5f, b2Vec2(0.0f, 18.0f), 0.0);
			boxBody->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(32.0f, 0.5f, b2Vec2(0.0f, -18.0f), 0.0);
			boxBody->CreateFixture(&shape, 5.0f);
		}

		{
			// Add in abody for hour and minute.
			addBox(3.0, 3.0);
			addBox(2.8, 2.8);
		}
	}

    b2Body* addBox(float x=0.6, float y=0.6)
	{
		// Create a rectangular box.
		b2BodyDef boxBody;
		boxBody.position.Set(0.0,20.0);
		boxBody.type = b2_dynamicBody;
		boxBody.angularVelocity = b2_pi;
		boxBody.linearVelocity = b2Vec2(0.0,0.0);

		b2Body* box = m_world->CreateBody(&boxBody);

		// Give it a shape.
		b2PolygonShape boxShape;
		boxShape.SetAsBox(x,y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = 0.8f;
		fixtureDef.friction = 0.5f;
		fixtureDef.restitution = 0.4f;

		box->CreateFixture(&fixtureDef);

		return box;
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
    		case GLFW_KEY_B:
			{
				addBox();
			}
			break;
    		case GLFW_KEY_K:
			{
				// Rotate gravity to left.
				m_angleSteps--;
        		m_world->SetGravity(getGravityVector());
			}
			break;
    		case GLFW_KEY_L:
			{
				// Rotate gravity to right.
				m_angleSteps++;
		        m_world->SetGravity(getGravityVector());
			}
			break;

		}
	}

	void Step(Settings& settings) override
	{
		g_debugDraw.DrawString(5, m_textLine, "Keys: box = b");
		m_textLine += m_textIncrement;
		
        auto t =  std::chrono::system_clock::now();
        auto t1 = std::chrono::system_clock::to_time_t(t);
		if ((t1 - m_time) > 1) {
			m_time=t1;

			m_boxes.push_front(addBox());

            if (m_boxes.size() > m_boxCount) {
				m_world->DestroyBody(m_boxes.back());
				m_boxes.pop_back();
			}
		}
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new PHTest2;
	}

};

static int testIndex = RegisterTest("PHTest", "Test2", PHTest2::Create);
