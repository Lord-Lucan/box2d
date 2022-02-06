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

#include "test.h"

class PHTest1 : public Test
{
public:

	PHTest1()
	{
		{
			// Create the body definition and set body properties.
			b2BodyDef groundBodyDef;
			groundBodyDef.position.Set(0.0f, -10.0f);

			// Actually create the body in the world.
			b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

			// Create the ground body shape.
			b2PolygonShape groundBox;
			groundBox.SetAsBox(30.0f, 10.0f);

			// Bind the shape to the body to create a fixture.
			groundBody->CreateFixture(&groundBox, 0.0f);
		}

		{
			// Create a static body for the anchor point.
			b2BodyDef bodyDefAnchor;
			bodyDefAnchor.position.Set(0.0f, 10.0f);
			b2Body* anchor = m_world->CreateBody(&bodyDefAnchor);

			// Create a pivoted see saw.
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(0.0f, 10.0f);
			b2Body* body = m_world->CreateBody(&bodyDef);

            // Now create a revolute joint betweent the two bodies.
			b2RevoluteJointDef jointDef;
		    jointDef.Initialize(anchor, body, anchor->GetWorldCenter());
			jointDef.upperAngle = b2_pi * 0.08;
			jointDef.lowerAngle = -b2_pi * 0.08;
			jointDef.enableLimit = true;
		    m_world->CreateJoint(&jointDef);

			b2PolygonShape box;
			box.SetAsBox(20.0, 0.1f);
            body->CreateFixture(&box, 0.1f);
			box.SetAsBox(0.1f, 0.5f, b2Vec2(20.0f, 0.4f), 0.0);
			body->CreateFixture(&box, 0.1f);
		}

		for(int i=0; i<2; ++ i)
		{
			// Now the ball.
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(i, 20.0f);
			bodyDef.angularVelocity = 0.0f;
			bodyDef.linearVelocity = b2Vec2(5.0,0.0);
			b2Body* body = m_world->CreateBody(&bodyDef);

			b2CircleShape ball;
			ball.m_radius = 0.5f;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &ball;
			fixtureDef.density = 0.8f;
			fixtureDef.friction = 0.5f;
			fixtureDef.restitution = 0.4f;

			body->CreateFixture(&fixtureDef);
		}
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_B:
			{
			// Now the ball.
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(-18.0f, 20.0f);
			b2Body* body = m_world->CreateBody(&bodyDef);

			b2CircleShape ball;
			ball.m_radius = 0.5f;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &ball;
			fixtureDef.density = 0.8f;
			fixtureDef.friction = 0.5f;
			fixtureDef.restitution = 0.4f;

			body->CreateFixture(&fixtureDef);
			}
			break;

		}
	}

	void Step(Settings& settings) override
	{
		g_debugDraw.DrawString(5, m_textLine, "Keys: ball = b");
		m_textLine += m_textIncrement;
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new PHTest1;
	}

};

static int testIndex = RegisterTest("PHTest", "Test1", PHTest1::Create);
