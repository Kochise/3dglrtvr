import sys
import os
import re

from subprocess import Popen, PIPE, call

def check_command( command ):
    result = True
    try:
        p = Popen( [ command ], stdout = PIPE, stderr = PIPE)
        out, err = p.communicate()
    except:
        result = False
    finally:
        pass

    return result

def create_directory_if_not_exists( dir_name ):
    if not os.path.exists( dir_name ):
        os.makedirs( dir_name )

class SWFExtractor:
    def __init__(self):
        self.tool_name='swfextract.exe'
        self.is_available=check_command(self.tool_name)

    def is_extractor_ready(self):
        if not self.is_available:
            print( self.tool_name + " is not acessible")
            return False
        return True

    def get_ids(self, ids):
        id_list = [ int( x.group() ) for x in re.finditer(r' \d+(?=[,x])', ids) ]
        range_id = [ x.group() for x in re.finditer(r'\d+-\d+', ids) ]

        for _range in range_id:
            start, end = [ int( x.group() ) for x in re.finditer(r'\d+', _range) ]
            end = end + 1
            id_list.extend(range(start,end))

        id_list.sort()

        return id_list

    def get_images( self, package):
        command = [ self.tool_name, package]

        p = Popen(command, stdout = PIPE, stderr = PIPE)
        out, err = p.communicate()

        out_strings = out.decode("utf-8").splitlines()

        jpgs = []
        pngs = []
        err = None

        for str in out_strings:
            if str.startswith(' [-j]'):
                print( 'gettings JPG')
                jpgs = self.get_ids(str)
            elif str.startswith(' [-p]'):
                print( 'gettings PNG')
                pngs = self.get_ids(str)

        if (len(jpgs) == 0) and ( len(pngs) == 0):
            err = 'There are no images in SWF file'

        return jpgs, pngs, err

    def extract_jpg(self, swf_file, path, id):
        file_name = os.path.join(path, id+'.jpg')

        command = [self.tool_name, '-j', id, '-o', file_name, swf_file]
        p = Popen( command, stderr = PIPE, stdout = PIPE)
        out, err = p.communicate()

        if err:
            print( err )

    def extract_png(self, swf_file, path, id):
        file_name = os.path.join(path, id+'.png')

        command = [self.tool_name, '-p', id, '-o', file_name, swf_file]
        p = Popen( command, stderr = PIPE, stdout = PIPE)
        out, err = p.communicate()

        if err:
            print( err )


    def extract_images(self, package, out_path=None):
        if not self.is_available:
            return False

        if not os.path.exists(package):
            print('SWF file not found: ' + package)
            return False

        if out_path == None:
            out_path = os.getcwd()

        print( 'Out set to: ' + out_path)

        jpg_list, png_list, err = self.get_images(package)
        if err:
            print( err )
            return False

        if len(jpg_list) != 0:
            jpg_path = os.path.join(out_path,'JPG')
            create_directory_if_not_exists(jpg_path)

            for id in jpg_list:
                self.extract_jpg(package, jpg_path, str(id))

        if len(png_list) != 0:
            png_path = os.path.join(out_path,'PNG')
            create_directory_if_not_exists(png_path)

            for id in png_list:
                self.extract_png(package, png_path, str(id))

        return True

def extract_images_from_swf(package, out_path=None):
    extractor = SWFExtractor()

    if not extractor.is_extractor_ready():
        return False

    if not extractor.extract_images(package, out_path):
        print( 'Failed to extract images')
        return False

    return True

def print_usage():
    print("Usage:")
    print( __file__ + " swf_file [output dir]")

if __name__ == '__main__':
    args = len(sys.argv)
    if (args < 2) or (args > 3):
        print_usage()
    else:
        if args == 2:
            extract_images_from_swf( sys.argv[1] )
        else:
            extract_images_from_swf( sys.argv[1], sys.argv[2] )