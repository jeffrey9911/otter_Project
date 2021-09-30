/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CPathAnimator.cpp
Simple component for animating an object along a path.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CPathAnimator.h"

#include<iostream>

namespace nou
{
	CPathAnimator::CPathAnimator(Entity& owner)
	{
		m_owner = &owner;

		m_segmentIndex = 0;
		m_segmentTimer = 0.0f;

		m_segmentTravelTime = 1.0f;
		m_mode = PathSampler::PathMode::LERP;
	}

	void CPathAnimator::SetMode(PathSampler::PathMode mode)
	{
		m_mode = mode;
		m_segmentIndex = 0;
	}

	void CPathAnimator::Update(const PathSampler::KeypointSet& keypoints, float deltaTime)
	{
		// TODO: Implement this function
		// Steps to follow:
		// Make sure we have points to use
		int kPointSize = keypoints.size(); // get the size of points

		if (kPointSize < 1) // if no waypoints, no action
		{
			return;
		}

		int maxSegInd = kPointSize - 1; // define the max segment index it would be

		// Increment our t value
		m_segmentTimer += deltaTime / 10; // increase segment timer. divide 10 to slower the time increment

		// Handle switching segments each time t = 1
		if (kPointSize == 2) // if 2 waypoints, reset segment index
		{
			m_segmentIndex = 1;
		}

		if (m_segmentTimer >= m_segmentTravelTime) // segment timer cannot be bigger than travel time which is 1
		{
			m_segmentTimer -= 1.0f;

			if (m_segmentIndex >= maxSegInd) // segment index cannot be bigger than the max
			{
				m_segmentIndex = 1; // if reached max, reset to 1
			}
			else
			{
				m_segmentIndex++; // if not reached max, plus 1
			}
		}

		// Work out t using segment timer and segment travel time
		// Ensure we have at least 2 points to LERP along
		// Need at least 2 points for 1 segment

		// Determine the indices of the points to LERP along (in our vector of key points)
		glm::vec3 duckPos = (m_owner->transform.m_pos); // get the position of the duck right now
		
		/* //Debuging
		std::cout << "m_SegmentIndex:" << m_segmentIndex
			<< "\tkPointSize:" << kPointSize 
			<< "\tm_SegmentTimer:" << m_segmentTimer
			<< std::endl;
		*/


		// Perform LERP using function in PathSampler
		//PathSampler::LERP(&keypoints[ind_1], &keypoints[ind_2], m_segmentTimer);
		glm::vec3 lerpPos; 
		if (kPointSize == 1) // if there is only one point, there is no segment, reset segment index to 0
		{
			m_segmentIndex = 0;
			lerpPos = PathSampler::LERP(duckPos, keypoints[m_segmentIndex]->transform.m_pos, m_segmentTimer);
				// no segment, so lerp from duck position to keypoints[0]
		}
		else
		{	// otherwise, lerp from segment index -1 to segment index
			lerpPos = PathSampler::LERP(keypoints[m_segmentIndex -1]->transform.m_pos, keypoints[m_segmentIndex]->transform.m_pos, m_segmentTimer);
		}

		m_owner->transform.m_pos = lerpPos; // give the lerped vector to the owner which is the duck
	}
}