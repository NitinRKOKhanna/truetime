function ttwnetwork_callback(cb,mask_values)

% Copyright (c) 2009 Lund University
%
% Written by Anton Cervin, Dan Henriksson and Martin Ohlin,
% Department of Automatic Control LTH, Lund University, Sweden.
%   
% This file is part of Truetime 2.0 beta.
%
% Truetime 2.0 beta is free software: you can redistribute it and/or
% modify it under the terms of the GNU General Public License as
% published by the Free Software Foundation, either version 3 of the
% License, or (at your option) any later version.
%
% Truetime 2.0 beta is distributed in the hope that it will be useful, but
% without any warranty; without even the implied warranty of
% merchantability or fitness for a particular purpose. See the GNU
% General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with Truetime 2.0 beta. If not, see <http://www.gnu.org/licenses/>

%%% Mask change callback

if nargin == 0
  mask_visibilities = get_param(gcb, 'MaskVisibilities');
  mask_values = get_param(gcb, 'MaskValues');
  if strcmp(mask_values{8}, 'default')
    mask_visibilities{9} = 'on';
    mask_visibilities{10} = 'off';
  else
    mask_visibilities{9} = 'off';
    mask_visibilities{10} = 'on';
  end
  set_param(gcb, 'MaskVisibilities', mask_visibilities)
  return
end

% Keep track of number of output ports 
nop = 0;

% Check if Network number or Number of nodes has changed
nwnbr = mask_values{2};
if (nwnbr < 1) | ~isequal(abs(round(nwnbr)),nwnbr)
  errordlg('Network number: wrong format')
  return
end
N = mask_values{3};
if (N < 1) | ~isequal(abs(round(N)),N)
  errordlg('Number of nodes: wrong format')
  return
end
% Remove all "From" blocks and associated lines
Nold = str2num(get_param([cb '/SndMux'],'Inputs'));
for k = 1:Nold
  delete_line(cb, ['From' num2str(k) '/1'], ['SndMux/' num2str(k)]);
  delete_block([cb '/From' num2str(k)]);
end
% Set new Mux size
set_param([cb '/SndMux'],'Inputs',num2str(N));
% Add new "From" blocks and associated lines
for k = 1:N
  name = [cb '/From' num2str(k)];
  gototag = ['snd' num2str(nwnbr) '_' num2str(k)];
  position = [10 k*20 70 k*20+18];
  add_block('built-in/From',name,'Position',position,'GotoTag',gototag,'ShowName','off');
  add_line(cb,['From' num2str(k) '/1'],['SndMux/' num2str(k)]);
end
% Remove all "Goto" blocks and associated lines
Nold = str2num(get_param([cb '/RcvDemux'],'Outputs'));
for k = 1:Nold
  delete_line(cb, ['RcvDemux/' num2str(k)], ['Goto' num2str(k) '/1']);
  delete_block([cb '/Goto' num2str(k)]);
end
% Set new Demux size
set_param([cb '/RcvDemux'],'Outputs',num2str(N));
% Add new "Goto" blocks and associated lines
for k = 1:N
  name = [cb '/Goto' num2str(k)];
  gototag = ['rcv' num2str(nwnbr) '_' num2str(k)];
  position = [710 k*20 770 k*20+18];
  add_block('built-in/Goto',name,'Position',position,'GotoTag',gototag,'ShowName','off','TagVisibility','global');
  add_line(cb,['RcvDemux/' num2str(k)],['Goto' num2str(k) '/1']);
end

% Check if Show X output port have changed
iixx = [16 17];
xxnames = {'Schedule','P'};
for ix = iixx
  i = find(ix==iixx);
  block = [cb '/' xxnames{i}];
  oldblocktype = get_param(block, 'BlockType');
  position = get_param(block, 'Position');
  orientation = get_param(block, 'Orientation');
  if isequal(mask_values{ix}, 1)
    newblocktype = 'Outport';
  else 
    newblocktype = 'Terminator';
  end
  if ~isequal(newblocktype,oldblocktype)
    delete_block(block)
    add_block(['built-in/' newblocktype], block, 'Position', position, 'Orientation', orientation)
  end
  if isequal(mask_values{ix}, 1)
    nop = nop + 1;
    set_param(block,'Port',num2str(nop))
  end
end
