from __future__ import division, print_function
from .measurement_images import MeasurementImage, ImageGroup

import libSEImplementation as cpp

apertures_for_image = {}


def add_aperture_photometry(target, apertures):
    if not isinstance(target, list):
        target = [target]
    if not isinstance(apertures, list):
        apertures = [apertures]

    apertures = [float(a) for a in apertures]
    outputs = []

    for t in target:
        if not isinstance(t, MeasurementImage):
            raise Exception('Only MeasurementImage supported as targets, got {}'.format(type(t)))
        else:
            if t.id in apertures_for_image:
                raise Exception('Apertures already set for the image {}'.format(t.id))
            apertures_for_image[t.id] = cpp.Aperture(apertures)
            outputs.append(apertures_for_image[t.id])

    return outputs