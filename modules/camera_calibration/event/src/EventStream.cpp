//
// Created by huangkun on 2020/9/15.
//

#include <exception>
#include <iostream>

#include <opengv2/event/EventStream.hpp>

#include <metavision/sdk/driver/camera.h>
#include <metavision/sdk/base/events/event_cd.h>

opengv2::EventStream::EventStream(const std::string &binFilePath) {
    is_.open(binFilePath, std::ifstream::binary | std::ifstream::in);
    if (is_.is_open()) {
        istreamIterator_ = std::istream_iterator<Event>(is_);
    } else {
        throw std::invalid_argument("No such file: " + binFilePath);
    }
}

opengv2::EventStream::~EventStream() {
    if (is_.is_open()) {
        is_.close();
    }
}

void opengv2::EventStream::txt2bin(const std::string &txtFilePath, double timeMagnitude,
                                   long long timeBase_in, long long endTime_in) {
    std::ifstream is(txtFilePath, std::ifstream::in);
    if (is.is_open()) {
        auto lastIndex = txtFilePath.find_last_of('.');
        std::ofstream os(txtFilePath.substr(0, lastIndex) + ".bin",
                         std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
        long long counter = 0;
        long long timeStamp, timeBase = timeBase_in, endTime = endTime_in;
        double x, y;
        bool polarity;
        while (is.good()) {
            is >> timeStamp >> x >> y >> polarity;

            if (counter == 0 && timeBase_in == std::numeric_limits<long long>::min())
                timeBase = timeStamp;

            if (endTime != std::numeric_limits<long long>::min() && timeStamp > endTime)
                break;
            double t = ((timeStamp - timeBase) * timeMagnitude); // convert to second
            if (t < 0)
                continue;
            os.write((char *) &t, sizeof(t));
            os.write((char *) &x, sizeof(x));
            os.write((char *) &y, sizeof(y));
            os.write((char *) &polarity, sizeof(polarity));

            counter++;
        }

        std::cout << counter << " data processed." << std::endl;
        std::cout.precision(30);
        std::cout << "TimeBase :" << timeBase << std::endl;
        std::cout.precision(15);
        std::cout << "Last data is :" << timeStamp << " " << x << " " << y << " " << polarity << std::endl;
        std::cout << "Duration :" << ((timeStamp - timeBase) * timeMagnitude) << std::endl;

        is.close();
        os.close();
    } else {
        throw std::invalid_argument("No such file: " + txtFilePath);
    }
}

void opengv2::EventStream::raw2bin(const std::string &rawFilePath) {
    Metavision::Camera cam = Metavision::Camera::from_file(rawFilePath);

    auto lastIndex = rawFilePath.find_last_of('.');
    std::ofstream os(rawFilePath.substr(0, lastIndex) + ".bin",
                     std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
    cam.cd().add_callback([&os](const Metavision::EventCD *begin, const Metavision::EventCD *end) {

    double t;
    double x;
    double y;
    bool polarity;
    for (const Metavision::EventCD *ev = begin; ev != end; ++ev) {
      t = ev->t /*us*/ / 1e6; // Convert time from [us] to [s]
      x = ev->x;
      y = ev->y;
      polarity = ev->p;

      os.write((char *) &t, sizeof(t));
      os.write((char *) &x, sizeof(x));
      os.write((char *) &y, sizeof(y));
      os.write((char *) &polarity, sizeof(polarity));
    }
  });

    cam.start();

    while (cam.is_running()) {}

    cam.stop();
    os.close();
}
