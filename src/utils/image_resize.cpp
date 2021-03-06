// -*- mode: c++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the JSK Lab nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#include "edgetpu_roscpp/image_resize.h"
#include <iostream>

void resize(const cv::Mat& input_img, const cv::Size required_size, bool keep_aspect_ratio, cv::Mat& output_img, CvSize2D32f& ratio)
{
  if(keep_aspect_ratio)
    {
      cv::Size new_size = required_size;
      double resampling_ratio = std::min((double)required_size.width / input_img.size().width,
                                         (double)required_size.height / input_img.size().height);


      new_size = cv::Size(input_img.size().width * resampling_ratio, input_img.size().height * resampling_ratio);
      std::cout << "[" << input_img.size().width << ", " << input_img.size().height << "] -> "
                << "[" << new_size.width << ", " << new_size.height << "]" << std::endl;

      cv::Mat resized_img;
      cv::resize(input_img, resized_img, new_size);

      int delta_w = required_size.width - new_size.width;
      int delta_h = required_size.height - new_size.height;
      cv::copyMakeBorder(resized_img, output_img, 0, delta_h, 0, delta_w, cv::BORDER_CONSTANT); // https://docs.opencv.org/3.4/dc/da3/tutorial_copyMakeBorder.html

      ratio = CvSize2D32f((double)new_size.width / required_size.width, (double)new_size.height / required_size.height);
    }
  else
    {
      cv::resize(input_img, output_img, required_size);
      ratio = CvSize2D32f(1, 1);
    }
}
